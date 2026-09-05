import { useMemo } from "react";
import { useBleStore, useDevices } from "@/entities/bluetooth";
import { useScanner } from "../hooks/useScanner";
import { DeviceList } from "../ui/DeviceList";
import { ScannerControls } from "../ui/ScannerControls";

export const BluetoothScannerPage: React.FC = () => {
	const devices = useDevices();
	const clearDevices = useBleStore((state) => state.clearDevices);

	const { isActive, isBlocked, isPending, toggleScan } = useScanner();

	const statusText = useMemo(() => {
		if (!isActive) return "Scanning has not started";
		if (isActive && devices.length) return "Scanning for Bluetooth devices...";
		return `${devices.length} device${devices.length === 1 ? "" : "s"} found`;
	}, [isActive, devices]);

	return (
		<div className="mx-auto flex w-full max-w-4xl flex-col gap-6 p-6">
			<ScannerControls
				isBlocked={isBlocked}
				isScanning={isActive}
				isLoading={isPending}
				buttonLabel={isActive ? "Stop scanning" : "Start scanning"}
				statusText={statusText}
				onToggle={toggleScan}
				onClear={clearDevices}
			/>

			<DeviceList
				devices={devices}
				isScanning={isActive}
				hasDevices={devices.length > 0}
			/>
		</div>
	);
};
