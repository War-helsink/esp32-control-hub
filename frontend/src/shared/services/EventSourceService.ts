type EventCallback<T> = (data: T) => void;

export class EventSourceService<T> {
	private readonly url = "/events";
	private eventName: string;
	private eventSource: EventSource | null = null;
	private listeners = new Set<EventCallback<T>>();

	constructor(eventName: string) {
		this.eventName = eventName;
	}

	subscribe(callback: EventCallback<T>): () => void {
		this.listeners.add(callback);

		if (this.listeners.size === 1) {
			this.connect();
		}

		return () => {
			this.listeners.delete(callback);

			if (this.listeners.size === 0) {
				this.disconnect();
			}
		};
	}

	private connect() {
		if (this.eventSource) return;

		this.eventSource = new EventSource(this.url);

		this.eventSource.addEventListener(this.eventName, (event) => {
			try {
				const parsedData = JSON.parse(event.data) as T;
				this.notify(parsedData);
			} catch (error) {
				console.error(
					`[EventSourceService] Error parsing event "${this.eventName}":`,
					error,
				);
			}
		});
	}

	private disconnect() {
		if (this.eventSource) {
			this.eventSource.close();
			this.eventSource = null;
		}
	}

	private notify(data: T) {
		this.listeners.forEach((callback) => {
			callback(data);
		});
	}
}
