import { useEffect } from "react";
import { Outlet } from "react-router";
import {
	useWifiStore,
	type WifiClientFoundEvent,
	type WifiNetworkFoundEvent,
} from "@/entities/wifi";
import { eventSourceService } from "@/shared/services";

export const WifiLayout: React.FC = () => {
	useEffect(() => {
		const unsubscribeNetwork =
			eventSourceService.subscribe<WifiNetworkFoundEvent>(
				"wifi:network:found",
				(event) => {
					useWifiStore.getState().upsertNetwork(event.payload);
				},
			);
		const unsubscribeClient =
			eventSourceService.subscribe<WifiClientFoundEvent>(
				"wifi:client:found",
				(event) => {
					useWifiStore.getState().upsertClient(event.payload);
				},
			);

		return () => {
			unsubscribeNetwork();
			unsubscribeClient();
		};
	}, []);

	return <Outlet />;
};
