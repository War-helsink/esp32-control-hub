import { useMemo } from "react";
import { useBleStore } from "../store";

export function useDevices() {
	const devicesByAddress = useBleStore((state) => state.devicesByAddress);
	return useMemo(() => {
		return Object.values(devicesByAddress).sort((a, b) => b.rssi - a.rssi);
	}, [devicesByAddress]);
}
