import { useTransition } from "react";
import { WifiApi } from "../api/WifiApi";
import type { WifiNetwork } from "../types";
import { useWifiStore } from "../store";

export function useSelectNetwork() {
	const [isPending, startTransition] = useTransition();

	const selectNetwork = (network: WifiNetwork | null) => {
		if (isPending) {
			return;
		}

		startTransition(async () => {
			if (network) {
				const selectedNetwork = useWifiStore.getState().selectedNetwork;
				if (selectedNetwork?.bssid !== network.bssid) {
					await WifiApi.selectNetwork({ ...network });
					return;
				}
			}
			await WifiApi.clearSelectedNetwork();
		});
	};

	return {
		selectNetwork,
		isPending,
	};
}
