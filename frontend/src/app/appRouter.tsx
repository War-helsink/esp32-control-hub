import { createBrowserRouter, Navigate } from "react-router";
import { DashboardPage } from "@/pages/dashboard";
import {
	BluetoothLayout,
	BluetoothJammerPage,
	BluetoothScannerPage,
} from "@/pages/bluetooth";
import {
	WifiLayout,
	WifiBeaconPage,
	WifiDeauthPage,
	WifiScannerPage,
	WifiHandshakePage,
	WifiPmkidPage,
} from "@/pages/wifi";
import { NotFound } from "@/pages/not-found";

import { BaseLayout } from "./layouts/BaseLayout";

export const appRouter = createBrowserRouter([
	{
		element: <BaseLayout />,
		children: [
			{ path: "/", element: <DashboardPage /> },
			{
				path: "/bluetooth",
				element: <BluetoothLayout />,
				children: [
					{ index: true, element: <Navigate to="scanner" replace /> },
					{ path: "scanner", element: <BluetoothScannerPage /> },
					{ path: "jammer", element: <BluetoothJammerPage /> },
				],
			},
			{
				path: "/wifi",
				element: <WifiLayout />,
				children: [
					{ index: true, element: <Navigate to="scanner" replace /> },
					{ path: "scanner", element: <WifiScannerPage /> },
					{
						path: "attacks",
						children: [
							{ path: "deauth", element: <WifiDeauthPage /> },
							{
								path: "beacon",
								element: <WifiBeaconPage />,
							},
							{
								path: "handshake",
								element: <WifiHandshakePage />,
							},
							{
								path: "pmkid",
								element: <WifiPmkidPage />,
							},
						],
					},
				],
			},
			{
				path: "*",
				element: <NotFound />,
			},
		],
	},
]);
