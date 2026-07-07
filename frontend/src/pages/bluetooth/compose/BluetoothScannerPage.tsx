import { useBleScanner } from "../hooks/useBleScanner";
import { DeviceList } from "../ui/DeviceList";
import { ScannerControls } from "../ui/ScannerControls";

export const BluetoothScannerPage: React.FC = () => {
	const {
		isScanning,
		isLoading,
		devices,
		hasDevices,
		buttonLabel,
		statusText,
		toggleScanning,
		clearDevice,
	} = useBleScanner();

	return (
		<div className="mx-auto flex w-full max-w-4xl flex-col gap-6 p-6">
			<ScannerControls
				isScanning={isScanning}
				isLoading={isLoading}
				buttonLabel={buttonLabel}
				statusText={statusText}
				onToggle={toggleScanning}
				onClear={clearDevice}
			/>

			<DeviceList
				devices={devices}
				isScanning={isScanning}
				hasDevices={hasDevices}
			/>
		</div>
	);
};
