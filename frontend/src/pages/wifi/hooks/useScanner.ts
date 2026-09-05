import { toast } from "sonner";
import { useTransition } from "react";
import { useWifiStore, WifiApi } from "@/entities/wifi";
import { canStart, isRunning, useStatusStore } from "@/entities/status";

export function useScanner() {
	const isActive = useStatusStore((state) => isRunning(state, "wifi", "scan"));
	const isBlocked = useStatusStore((state) => !canStart(state, "wifi", "scan"));
	const [isPending, startTransition] = useTransition();

	function toggleScan() {
		if (isBlocked || isPending) {
			return;
		}

		startTransition(async () => {
			try {
				if (isActive) {
					await WifiApi.stopScanner();
					toast.success("Stopped successfully!");
					return;
				}

				await WifiApi.startScanner();

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
		toggleScan,
	};
}
