import { useTransition } from "react";
import { WifiApi } from "../api/WifiApi";
import type { WifiClient } from "../types";
import { useWifiStore } from "../store";

export function useSelectClient() {
	const [isPending, startTransition] = useTransition();

	const selectClient = (client: WifiClient | null) => {
		if (isPending) {
			return;
		}

		startTransition(async () => {
			if (client) {
				const selectedClient = useWifiStore.getState().selectedClient;
				if (selectedClient?.mac !== client.mac) {
					await WifiApi.selectClient({ ...client });
					return;
				}
			}
			await WifiApi.clearSelectedClient();
		});
	};

	return {
		selectClient,
		isPending,
	};
}
