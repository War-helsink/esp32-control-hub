import { Radio } from "lucide-react";
import { Badge } from "@/shared/ui";

interface JammerHeaderProps {
	isActive: boolean;
}

export const JammerHeader: React.FC<JammerHeaderProps> = ({ isActive }) => (
	<div className="flex flex-col sm:flex-row sm:items-center justify-between gap-4 border-b border-border pb-6 backdrop-blur-sm bg-background/20 rounded-xl p-4">
		<div className="space-y-1">
			<div className="flex items-center gap-2.5">
				<Radio
					className={`h-6 w-6 ${isActive ? "text-destructive animate-pulse" : "text-primary"}`}
				/>
				<h1 className="text-2xl md:text-3xl font-bold tracking-tight">
					BLE Jammer System
				</h1>
			</div>
			<p className="text-sm text-muted-foreground">
				Manage Bluetooth protocol suppression and transmission power.
			</p>
		</div>
		<div className="flex items-center">
			<Badge
				variant={isActive ? "destructive" : "secondary"}
				className="px-3 py-1 text-xs font-semibold tracking-wider uppercase"
			>
				{isActive ? "Signal Active" : "System Idle"}
			</Badge>
		</div>
	</div>
);
