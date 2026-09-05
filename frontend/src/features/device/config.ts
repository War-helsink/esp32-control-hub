import type { ActiveProcess } from "@/entities/status";

export const moduleLabels: Record<ActiveProcess["module"], string> = {
	wifi: "Wi-Fi",
	bluetooth: "Bluetooth",
};
