export const BleDeviceStatus = {
	Connectable: "connectable",
	NonConnectable: "non_connectable",
	Directed: "directed",
	Unknown: "unknown",
} as const;

export interface BleEvent {
	type: "device_found";
	payload: BleDevice;
}

export type BleDeviceStatus =
	(typeof BleDeviceStatus)[keyof typeof BleDeviceStatus];

export interface BleDevice {
	address: string;
	name: string;
	rssi: number;
	status: BleDeviceStatus;
}

export interface BleState {
	devicesByAddress: Record<string, BleDevice>;
	selectedDevice: BleDevice | null;
}

export interface BleActions {
	// Device
	selectDevice: (network: BleDevice | null) => void;
	upsertDevice: (network: BleDevice) => void;
	clearDevices: () => void;

	// Reset
	resetSelection: () => void;
	reset: () => void;
}

export type BleStore = BleActions & BleState;
