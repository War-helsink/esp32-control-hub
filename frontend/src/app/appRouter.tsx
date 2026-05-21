import { createBrowserRouter } from "react-router";
import { DashboardPage } from "@/pages/dashboard";
import BaseLayouts from "./layouts/BaseLayouts";

export const appRouter = createBrowserRouter([
	{
		element: <BaseLayouts />,
		children: [{ path: "/", element: <DashboardPage /> }],
	},
]);
