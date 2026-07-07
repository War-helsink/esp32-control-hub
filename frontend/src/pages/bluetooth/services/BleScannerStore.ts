import { EventSourceService } from "@/shared/services";
import { BleApi, type BleDevice } from "@/entities/bluetooth";

interface BleEvent {
	type: "device_found";
	payload: BleDevice;
}

export interface BleScannerState {
	isScanning: boolean;
	isLoading: boolean;
	devices: BleDevice[];
}

type StoreListener = (state: BleScannerState) => void;

class BleScannerStore {
	private state: BleScannerState = {
		isScanning: false,
		isLoading: false,
		devices: [],
	};

	private listeners = new Set<StoreListener>();
	private eventSource = new EventSourceService<BleEvent>("ble_general_update");
	private unsubscribeEvents: (() => void) | null = null;

	public subscribe(listener: StoreListener): () => void {
		this.listeners.add(listener);
		listener(this.state);
		return () => this.listeners.delete(listener);
	}

	public getState(): BleScannerState {
		return this.state;
	}

	private notify() {
		this.state = { ...this.state };
		this.listeners.forEach((listener) => {
			listener({ ...this.state });
		});
	}

	public startListening() {
		if (this.unsubscribeEvents) return;

		this.unsubscribeEvents = this.eventSource.subscribe((event) => {
			if (event.type !== "device_found") return;
			this.handleDeviceFound(event.payload);
		});
	}

	public stopListening() {
		if (this.unsubscribeEvents) {
			this.unsubscribeEvents();
			this.unsubscribeEvents = null;
		}
	}

	private handleDeviceFound(device: BleDevice) {
		const exists = this.state.devices.some(
			(item) => item.address === device.address,
		);

		if (exists) {
			this.state.devices = this.state.devices.map((item) =>
				item.address === device.address ? { ...item, ...device } : item,
			);
		} else {
			this.state.devices = [device, ...this.state.devices];
		}
		this.notify();
	}

	public clearDevice() {
		if (!this.state.isScanning && !this.state.isLoading) {
			this.state.devices = [];
			this.notify();
		}
	}

	public async toggleScanning() {
		this.state.isLoading = true;
		this.notify();

		try {
			if (this.state.isScanning) {
				await BleApi.stopScanning();
			} else {
				await BleApi.startScanning();
			}
			this.state.isScanning = !this.state.isScanning;
		} catch (error) {
			console.error("[BleScannerStore] Toggle scanning failed:", error);
		} finally {
			this.state.isLoading = false;
			this.notify();
		}
	}
}

export const bleScannerStore = new BleScannerStore();
