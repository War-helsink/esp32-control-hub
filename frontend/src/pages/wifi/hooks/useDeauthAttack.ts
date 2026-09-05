import { useTransition } from "react";
import { useWifiStore, WifiApi } from "@/entities/wifi";
import { canStart, isRunning, useStatusStore } from "@/entities/status";

export function useDeauthAttack() {
	const isActive = useStatusStore((state) =>
		isRunning(state, "wifi", "deauth"),
	);
	const isBlocked = useStatusStore(
		(state) => !canStart(state, "wifi", "deauth"),
	);
	const [isPending, startTransition] = useTransition();

	function toggleDeauthAttack() {
		if (isBlocked || isPending) {
			return;
		}

		startTransition(async () => {
			const selectedNetwork = useWifiStore.getState().selectedNetwork;

			if (!selectedNetwork) return;

			if (isActive) {
				await WifiApi.stopDeauth();
				return;
			}

			await WifiApi.startDeauth();
		});
	}

	return {
		isActive,
		isBlocked,
		isPending,
		toggleDeauthAttack,
	};
}
