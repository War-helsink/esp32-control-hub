import { useEffect } from "react";
import { Outlet } from "react-router";
import {
	useWifiStore,
	type WifiClientFoundEvent,
	type WifiNetworkFoundEvent,
} from "@/entities/wifi";
import { eventSourceService } from "@/shared/services";
import type { StatusState } from "../types";
import { setStatus } from "../utils";

export const StatusLayout: React.FC = () => {
	useEffect(() => {
		const unsubscribe = eventSourceService.subscribe<StatusState>(
			"status",
			(event) => {
				return setStatus(event);
			},
		);

		const unsubscribeNetworkSelect =
			eventSourceService.subscribe<WifiNetworkFoundEvent | null>(
				"wifi:network:select",
				(event) => {
					if (event) {
						return useWifiStore.getState().selectNetwork(event.payload);
					}
					useWifiStore.getState().selectNetwork(null);
				},
			);

		const unsubscribeClientSelect =
			eventSourceService.subscribe<WifiClientFoundEvent | null>(
				"wifi:client:select",
				(event) => {
					if (event) {
						return useWifiStore.getState().selectClient(event.payload);
					}
					useWifiStore.getState().selectClient(null);
				},
			);

		return () => {
			unsubscribe();
			unsubscribeNetworkSelect();
			unsubscribeClientSelect();
		};
	}, []);

	return <Outlet />;
};
