import { useEffect } from "react";
import { Outlet } from "react-router";
import { eventSourceService } from "@/shared/services";
import { useBleStore, type BleEvent } from "@/entities/bluetooth";

export const BluetoothLayout: React.FC = () => {
	useEffect(() => {
		const unsubscribe = eventSourceService.subscribe<BleEvent>(
			"ble:general:update",
			(event) => {
				useBleStore.getState().upsertDevice(event.payload);
			},
		);

		return unsubscribe;
	}, []);

	return <Outlet />;
};
