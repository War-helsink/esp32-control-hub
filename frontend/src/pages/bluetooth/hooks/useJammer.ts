import { toast } from "sonner";
import { useState, useTransition } from "react";
import { BleApi } from "@/entities/bluetooth";
import { canStart, isRunning, useStatusStore } from "@/entities/status";
import { useDebounceCallback } from "@/shared/hooks/use-debounce";

export function useJammer() {
	const isActive = useStatusStore((state) =>
		isRunning(state, "bluetooth", "jammer"),
	);
	const isBlocked = useStatusStore(
		(state) => !canStart(state, "bluetooth", "jammer"),
	);
	const [isPending, startTransition] = useTransition();

	function toggleJammer() {
		if (isBlocked || isPending) {
			return;
		}

		startTransition(async () => {
			if (!isActive) {
				await BleApi.startJammer();
				toast.success("Successfully started!");
				return;
			}
			await BleApi.stopJammer();
			toast.success("Stopped successfully!");
			return;
		});
	}

	const [power, setPower] = useState<number>(4);

	const setJammerConfig = useDebounceCallback(async (value: number) => {
		try {
			await BleApi.setJammerConfig(value);
			toast.success(`Successfully changed to ${value}.`);
		} catch (error) {
			toast.error("An error occurred!");
			console.error("Failed to update power:", error);
		}
	}, 500);

	const powerChange = async (value: number[]) => {
		const newPower = value[0];
		setPower(newPower);
		setJammerConfig(newPower);
	};

	return {
		isActive,
		isBlocked,
		isPending,
		toggleJammer,
		power,
		powerChange,
	};
}
