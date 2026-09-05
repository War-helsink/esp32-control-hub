import { Router, Smartphone } from "lucide-react";
import type { WifiClient } from "@/entities/wifi";
import { TableCell, TableRow } from "@/shared/ui";
import { formatMacAddress } from "../utils";
import { SignalIndicator } from "./SignalIndicator";
import { cn } from "@/shared/lib/utils";

interface ClientTableRowProps {
	client: WifiClient;
	isSelected: boolean;
	onSelect: (client: WifiClient) => void;
}

export const ClientTableRow: React.FC<ClientTableRowProps> = ({
	client,
	onSelect,
	isSelected,
}) => {
	return (
		<TableRow
			className={cn(
				"cursor-pointer transition-colors",
				isSelected && "bg-primary/5 hover:bg-primary/10",
			)}
			onClick={() => onSelect(client)}
		>
			<TableCell>
				<div className="flex min-w-44 items-center gap-3">
					<div
						className={cn(
							"flex size-9 shrink-0 items-center justify-center rounded-md border bg-background",
							isSelected && "border-primary/30 bg-primary/5 text-primary",
						)}
					>
						<Smartphone className="size-4" />
					</div>

					<div>
						<p className="font-mono text-xs font-medium">
							{formatMacAddress(client.mac)}
						</p>

						<p className="text-xs text-muted-foreground">Wireless client</p>
					</div>
				</div>
			</TableCell>

			<TableCell>
				<SignalIndicator rssi={client.rssi} />
			</TableCell>

			<TableCell className="pr-4">
				<div className="flex justify-end items-center gap-2">
					<Router className="size-4 text-muted-foreground" />

					<span className="font-mono text-xs text-muted-foreground">
						{formatMacAddress(client.bssid)}
					</span>
				</div>
			</TableCell>
		</TableRow>
	);
};
