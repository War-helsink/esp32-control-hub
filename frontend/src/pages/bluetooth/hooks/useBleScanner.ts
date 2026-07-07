import { useSyncExternalStore, useEffect, useMemo } from "react";
import { bleScannerStore } from "../services/BleScannerStore";

export function useBleScanner() {
	const state = useSyncExternalStore(
		(cb) => bleScannerStore.subscribe(cb),
		() => bleScannerStore.getState(),
	);

	useEffect(() => {
		bleScannerStore.startListening();
		return () => bleScannerStore.stopListening();
	}, []);

	const hasDevices = state.devices.length > 0;

	const buttonLabel = state.isScanning ? "Stop scanning" : "Start scanning";

	const statusText = useMemo(() => {
		if (!state.isScanning) return "Scanning has not started";
		if (state.isScanning && !hasDevices)
			return "Scanning for Bluetooth devices...";
		return `${state.devices.length} device${state.devices.length === 1 ? "" : "s"} found`;
	}, [state.isScanning, hasDevices, state.devices.length]);

	return {
		...state,
		hasDevices,
		buttonLabel,
		statusText,
		toggleScanning: () => bleScannerStore.toggleScanning(),
		clearDevice: () => bleScannerStore.clearDevice(),
	};
}
