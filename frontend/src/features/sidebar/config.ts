import { BluetoothSearching, LayoutList, RadioOff } from "lucide-react";
import { routesConfig } from "@/shared/config";
import type { NavMenu } from "./types";

export const navMain: NavMenu[] = [
	{
		title: "Dashboard",
		url: routesConfig.dashboard,
		icon: LayoutList,
	},
	{
		title: "Bluetooth",
		items: [
			{
				title: "Scanner",
				url: routesConfig.bluetoothScanner,
				icon: BluetoothSearching,
			},
			{
				title: "Jammer",
				url: routesConfig.bluetoothJammer,
				icon: RadioOff,
			},
		],
	},
	{
		title: "Wifi",
		items: [],
	},
	{
		title: "Radio Frequency",
		items: [],
	},
];
