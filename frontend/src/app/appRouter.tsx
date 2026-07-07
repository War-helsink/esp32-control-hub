import { createBrowserRouter, Navigate } from "react-router";
import { DashboardPage } from "@/pages/dashboard";
import { NotFound } from "@/pages/not-found";
import { BluetoothJammerPage, BluetoothScannerPage } from "@/pages/bluetooth";
import BaseLayouts from "./layouts/BaseLayouts";

export const appRouter = createBrowserRouter([
	{
		element: <BaseLayouts />,
		children: [
			{ path: "/", element: <DashboardPage /> },
			{
				path: "/bluetooth",
				children: [
					{ index: true, element: <Navigate to="scanner" replace /> },
					{ path: "scanner", element: <BluetoothScannerPage /> },
					{ path: "jammer", element: <BluetoothJammerPage /> },
				],
			},
			{
				path: "*",
				element: <NotFound />,
			},
		],
	},
]);
