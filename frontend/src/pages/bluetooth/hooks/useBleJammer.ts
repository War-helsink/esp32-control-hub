import { useState, useEffect } from "react";
import { BleApi } from "@/entities/bluetooth";

export function useBleJammer() {
	const [isActive, setIsActive] = useState<boolean>(false);
	const [power, setPower] = useState<number>(4);
	const [isLoading, setIsLoading] = useState<boolean>(false);

	useEffect(() => {
		async function fetchStatus() {
			const data = await BleApi.getJammerStatus();
			setIsActive(data.active);
			setPower(data.power);
		}

		fetchStatus();
	}, []);

	const toggleJammer = async () => {
		setIsLoading(true);
		try {
			if (isActive) {
				await BleApi.stopJammer();
				setIsActive(false);
			} else {
				await BleApi.startJammer();
				setIsActive(true);
			}
		} catch (error) {
			console.error("Failed to toggle jammer:", error);
		} finally {
			setIsLoading(false);
		}
	};

	const handlePowerChange = async (value: number[]) => {
		const newPower = value[0];
		setPower(newPower);
		try {
			await BleApi.setJammerConfig(newPower);
		} catch (error) {
			console.error("Failed to update power:", error);
		}
	};

	return {
		isActive,
		power,
		isLoading,
		toggleJammer,
		handlePowerChange,
	};
}
