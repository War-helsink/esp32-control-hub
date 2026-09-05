import { LoaderCircle, Radio, Square } from "lucide-react";
import { ScannerCardHeader, NetworksTable } from "@/features/wifi";
import { useNetworks, useSelectNetwork, useWifiStore } from "@/entities/wifi";
import { Badge, Button, Card, CardContent, CardFooter } from "@/shared/ui";
import { cn } from "@/shared/lib/utils";

import { useScanner } from "../hooks/useScanner";

export const NetworksScanner: React.FC = () => {
	const { isActive, isPending, isBlocked, toggleScan } = useScanner();

	const networks = useNetworks();
	const selectedNetwork = useWifiStore((store) => store.selectedNetwork);
	const { selectNetwork } = useSelectNetwork();

	return (
		<Card className="overflow-hidden shadow-sm gap-0">
			<ScannerCardHeader
				title="Network Scanner"
				start={
					<Badge
						variant="outline"
						className={cn(
							"gap-1.5 font-normal",
							isActive
								? "border-emerald-200 bg-emerald-50 text-emerald-700"
								: "text-muted-foreground",
						)}
					>
						<span
							className={cn(
								"size-2 rounded-full",
								isActive
									? "animate-pulse bg-emerald-500"
									: "bg-muted-foreground/40",
							)}
						/>

						{isActive ? "Scanning" : "Ready"}
					</Badge>
				}
				end={
					<Button
						type="button"
						variant={isActive ? "outline" : "default"}
						disabled={isPending || isBlocked}
						onClick={toggleScan}
						className="w-full shrink-0 sm:w-auto"
					>
						{isPending ? (
							<LoaderCircle className="animate-spin" />
						) : isActive ? (
							<Square className="fill-current" />
						) : (
							<Radio />
						)}

						{isPending
							? "Processing..."
							: isActive
								? "Stop Network Scan"
								: "Start Network Scan"}
					</Button>
				}
			/>

			<CardContent className="p-0">
				<NetworksTable
					networks={networks}
					selectedNetwork={selectedNetwork}
					onSelectNetwork={selectNetwork}
				/>
			</CardContent>

			<CardFooter className="justify-between border-t bg-muted/20 py-3 text-xs text-muted-foreground">
				<span>Showing {networks.length} networks</span>

				{selectedNetwork && (
					<span className="max-w-52 truncate">
						Selected:{" "}
						<strong className="font-medium text-foreground">
							{selectedNetwork.ssid || selectedNetwork.bssid}
						</strong>
					</span>
				)}
			</CardFooter>
		</Card>
	);
};
