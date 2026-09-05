import { useMemo } from "react";
import { useWifiStore } from "../store";

export function useNetworks() {
	const networksByBssid = useWifiStore((state) => state.networksByBssid);
	return useMemo(() => {
		return Object.values(networksByBssid).sort((a, b) => b.rssi - a.rssi);
	}, [networksByBssid]);
}
