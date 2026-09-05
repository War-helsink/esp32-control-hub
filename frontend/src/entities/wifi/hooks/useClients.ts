import { useMemo } from "react";
import { useWifiStore } from "../store";

export function useClients() {
	const clientsByMac = useWifiStore((state) => state.clientsByMac);
	return useMemo(() => {
		return Object.values(clientsByMac).sort((a, b) => b.rssi - a.rssi);
	}, [clientsByMac]);
}
