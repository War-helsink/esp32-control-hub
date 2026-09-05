import { toast } from "sonner";
import { useTransition } from "react";
import { canStart, isRunning, useStatusStore } from "@/entities/status";
import { BleApi, useBleStore } from "@/entities/bluetooth";

export function useScanner() {
	const isActive = useStatusStore((state) =>
		isRunning(state, "bluetooth", "scan"),
	);
	const isBlocked = useStatusStore(
		(state) => !canStart(state, "bluetooth", "scan"),
	);
	const [isPending, startTransition] = useTransition();

	function toggleScan() {
		if (isBlocked || isPending) {
			return;
		}

		startTransition(async () => {
			try {
				if (isActive) {
					await BleApi.stopScanning();
					toast.success("Stopped successfully!");
					return;
				}

				await BleApi.startScanning();

				useBleStore.getState().clearDevices();
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
