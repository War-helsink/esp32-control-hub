import { Signal } from "lucide-react";
import { BleDeviceStatus, type BleDevice } from "@/entities/bluetooth";
import { Badge } from "@/shared/ui";

interface DeviceRowProps {
	device: BleDevice;
}

const getStatusConfig = (status: BleDeviceStatus) => {
	switch (status) {
		case BleDeviceStatus.Connectable:
			return {
				label: "Connectable",
				variant: "default" as const,
				className:
					"bg-emerald-500 hover:bg-emerald-500/90 text-white dark:bg-emerald-600 dark:hover:bg-emerald-600/90",
			};
		case BleDeviceStatus.NonConnectable:
			return {
				label: "Non-connectable",
				variant: "destructive" as const,
				className: "",
			};
		case BleDeviceStatus.Directed:
			return {
				label: "Directed",
				variant: "secondary" as const,
				className:
					"bg-blue-500 hover:bg-blue-500/90 text-white dark:bg-blue-600",
			};
		case BleDeviceStatus.Unknown:
		default:
			return {
				label: "Unknown",
				variant: "outline" as const,
				className: "text-muted-foreground",
			};
	}
};

export const DeviceRow: React.FC<DeviceRowProps> = ({ device }) => {
	const { label, variant, className } = getStatusConfig(device.status);

	return (
		<div className="flex items-center justify-between rounded-lg border p-4 transition-colors hover:bg-muted/50">
			<div className="flex flex-col gap-1.5">
				<div className="flex items-center gap-2.5">
					<span className="font-medium tracking-tight">
						{device.name || "Unknown device"}
					</span>

					<Badge variant={variant} className={className}>
						{label}
					</Badge>
				</div>

				<div className="font-mono text-xs text-muted-foreground tracking-wider">
					{device.address}
				</div>
			</div>

			<div className="flex items-center gap-2 text-sm text-muted-foreground">
				<Signal className="h-4 w-4" />
				<span className="font-semibold text-foreground">
					{device.rssi ?? "N/A"}
				</span>
				<span className="text-xs text-muted-foreground/70 font-medium">
					dBm
				</span>
			</div>
		</div>
	);
};
