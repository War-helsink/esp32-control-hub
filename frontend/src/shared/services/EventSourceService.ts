type EventCallback<T> = (event: T) => void;
type Unsubscribe = () => void;

type UnknownEventCallback = EventCallback<unknown>;

class EventSourceService {
	private readonly url = "/events";

	private eventSource: EventSource | null = null;
	private readonly listeners = new Map<string, Set<UnknownEventCallback>>();
	private readonly eventHandlers = new Map<string, EventListener>();

	public subscribe<T>(
		eventName: string,
		callback: EventCallback<T>,
	): Unsubscribe {
		const listeners = this.getOrCreateListeners(eventName);

		listeners.add(callback as UnknownEventCallback);

		this.registerEventHandler(eventName);
		this.connect();

		let isSubscribed = true;

		return () => {
			if (!isSubscribed) {
				return;
			}

			isSubscribed = false;

			this.unsubscribe(eventName, callback as UnknownEventCallback);
		};
	}

	public disconnect(): void {
		if (!this.eventSource) {
			return;
		}

		this.eventSource.close();
		this.eventSource = null;
	}

	private connect(): void {
		if (this.eventSource) {
			return;
		}

		this.eventSource = new EventSource(this.url);

		this.eventHandlers.forEach((handler, eventName) => {
			this.eventSource?.addEventListener(eventName, handler);
		});

		this.eventSource.addEventListener("open", () => {
			console.info("[EventSourceService] Connected");
		});

		this.eventSource.addEventListener("error", (error) => {
			console.error("[EventSourceService] Connection error:", error);
		});
	}

	private getOrCreateListeners(eventName: string): Set<UnknownEventCallback> {
		const existingListeners = this.listeners.get(eventName);

		if (existingListeners) {
			return existingListeners;
		}

		const newListeners = new Set<UnknownEventCallback>();

		this.listeners.set(eventName, newListeners);

		return newListeners;
	}

	private registerEventHandler(eventName: string): void {
		if (this.eventHandlers.has(eventName)) {
			return;
		}

		const handler: EventListener = (event) => {
			if (!(event instanceof MessageEvent)) {
				return;
			}

			this.handleMessage(eventName, event);
		};

		this.eventHandlers.set(eventName, handler);

		this.eventSource?.addEventListener(eventName, handler);
	}

	private handleMessage(eventName: string, event: MessageEvent<string>): void {
		const parsedEvent: unknown = JSON.parse(event.data);

		this.notify(eventName, parsedEvent);
	}

	private notify(eventName: string, event: unknown): void {
		const listeners = this.listeners.get(eventName);

		if (!listeners) {
			return;
		}

		[...listeners].forEach((callback) => {
			callback(event);
		});
	}

	private unsubscribe(eventName: string, callback: UnknownEventCallback): void {
		const listeners = this.listeners.get(eventName);

		if (!listeners) {
			return;
		}

		listeners.delete(callback);

		if (listeners.size > 0) {
			return;
		}

		this.listeners.delete(eventName);
		this.unregisterEventHandler(eventName);

		if (this.listeners.size === 0) {
			this.disconnect();
		}
	}

	private unregisterEventHandler(eventName: string): void {
		const handler = this.eventHandlers.get(eventName);

		if (!handler) {
			return;
		}

		this.eventSource?.removeEventListener(eventName, handler);
		this.eventHandlers.delete(eventName);
	}
}

export const eventSourceService = new EventSourceService();
