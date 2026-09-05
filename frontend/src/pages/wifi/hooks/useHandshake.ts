import { toast } from "sonner";
import { useState, useTransition } from "react";
import { canStart, isRunning, useStatusStore } from "@/entities/status";
import { useWifiStore, WifiApi, type HandshakeType } from "@/entities/wifi";

export function useHandshake() {
	const [type, setType] = useState<HandshakeType>("passive");

	const isActive = useStatusStore((state) =>
		isRunning(state, "wifi", "handshake"),
	);
	const isBlocked = useStatusStore(
		(state) => !canStart(state, "wifi", "handshake"),
	);
	const [isPending, startTransition] = useTransition();

	function toggleHandshakeAttack() {
		if (isBlocked || isPending) {
			return;
		}

		startTransition(async () => {
			const selectedNetwork = useWifiStore.getState().selectedNetwork;
			if (!selectedNetwork) return;

			try {
				if (isActive) {
					await WifiApi.stopHandshake();
					toast.success("Stopped successfully!");
					return;
				}

				await WifiApi.startHandshake({
					type,
					ssid: selectedNetwork.ssid,
				});
				toast.success("Successfully started!");
			} catch {
				toast.error("An error occurred!");
			}
		});
	}

	return {
		type,
		changeType: setType,
		isActive,
		isBlocked,
		isPending,
		toggleHandshakeAttack,
	};
}
