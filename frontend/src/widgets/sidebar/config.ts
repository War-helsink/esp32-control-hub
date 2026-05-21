import { LayoutList } from "lucide-react";
import { routesConfig } from "@/shared/config";

export const navMain = [
	{
		title: "Dashboard",
		url: routesConfig.dashboard,
		icon: LayoutList,
	},
] as const;
