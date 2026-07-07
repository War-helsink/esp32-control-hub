export const BleDeviceStatus = {
	Connectable: "connectable",
	NonConnectable: "non_connectable",
	Directed: "directed",
	Unknown: "unknown",
} as const;

export type BleDeviceStatus =
	(typeof BleDeviceStatus)[keyof typeof BleDeviceStatus];

export interface BleDevice {
	address: string;
	name?: string;
	rssi?: number;
	status: BleDeviceStatus;
	timestamp?: number;
}
