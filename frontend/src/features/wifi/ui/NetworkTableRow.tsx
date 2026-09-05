import type { WifiNetwork } from "@/entities/wifi";
import { cn } from "@/shared/lib/utils";
import { Badge, TableCell, TableRow } from "@/shared/ui";
import { LockKeyhole, Wifi } from "lucide-react";
import { formatEncryption, formatMacAddress } from "../utils";
import { SignalIndicator } from "./SignalIndicator";

interface NetworkTableRowProps {
	network: WifiNetwork;
	isSelected: boolean;
	onSelect: (network: WifiNetwork) => void;
}

export const NetworkTableRow: React.FC<NetworkTableRowProps> = ({
	network,
	isSelected,
	onSelect,
}) => {
	return (
		<TableRow
			className={cn(
				"cursor-pointer transition-colors",
				isSelected && "bg-primary/5 hover:bg-primary/10",
			)}
			onClick={() => onSelect(network)}
		>
			<TableCell>
				<div className="flex min-w-44 items-center gap-3">
					<div
						className={cn(
							"flex size-9 shrink-0 items-center justify-center rounded-md border bg-background",
							isSelected && "border-primary/30 bg-primary/5 text-primary",
						)}
					>
						<Wifi className="size-4" />
					</div>

					<div className="min-w-0">
						<span className="max-w-52 truncate font-medium">
							{network.ssid || "Hidden network"}
						</span>
					</div>
				</div>
			</TableCell>

			<TableCell className="font-mono text-xs text-muted-foreground">
				{formatMacAddress(network.bssid)}
			</TableCell>

			<TableCell>
				<SignalIndicator rssi={network.rssi} />
			</TableCell>

			<TableCell>
				<Badge variant="secondary" className="font-mono">
					CH {network.channel}
				</Badge>
			</TableCell>

			<TableCell className="pr-4">
				<div className="flex justify-end items-center gap-2">
					<LockKeyhole className="size-3.5 text-muted-foreground" />

					<span className="whitespace-nowrap text-sm">
						{formatEncryption(network.encryption)}
					</span>
				</div>
			</TableCell>
		</TableRow>
	);
};
