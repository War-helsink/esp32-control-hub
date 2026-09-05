import { toast } from "sonner";
import { useTransition } from "react";
import { useWifiStore, WifiApi } from "@/entities/wifi";
import { canStart, isRunning, useStatusStore } from "@/entities/status";

export function useBeacon() {
	const isActive = useStatusStore((state) =>
		isRunning(state, "wifi", "beacon"),
	);
	const isBlocked = useStatusStore(
		(state) => !canStart(state, "wifi", "beacon"),
	);
	const [isPending, startTransition] = useTransition();

	function toggleBeacon() {
		if (isBlocked || isPending) {
			return;
		}

		startTransition(async () => {
			try {
				if (isActive) {
					await WifiApi.stopBeacon()
					toast.success("Stopped successfully!");
					return;
				}

				await WifiApi.startBeacon(1);

				useWifiStore.getState().clearNetworks();
				toast.success("Successfully started!");
			} catch {
				toast.error("An error occurred!");
			}
		});
	}

	return {
		isActive,
		isBlocked,
		isPending,
		toggleBeacon,
	};
}
