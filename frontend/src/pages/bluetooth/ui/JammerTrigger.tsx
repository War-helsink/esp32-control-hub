import { Power, ShieldAlert, ShieldCheck } from "lucide-react";
import { Button } from "@/shared/ui";

interface JammerTriggerProps {
	isActive: boolean;
	isLoading: boolean;
	onToggle: () => void;
}

export const JammerTrigger: React.FC<JammerTriggerProps> = ({
	isActive,
	isLoading,
	onToggle,
}) => (
	<div className="flex flex-col items-center justify-center flex-1 my-8">
		<div className="relative group">
			{isActive && (
				<span className="absolute inset-0 rounded-full bg-destructive/20 animate-ping scale-110 -z-50" />
			)}
			<Button
				onClick={onToggle}
				disabled={isLoading}
				variant={isActive ? "destructive" : "outline"}
				className={`w-40 h-40 md:w-48 md:h-48 rounded-full border-2 text-lg font-semibold flex flex-col gap-2 shadow-xl transition-all duration-300 ${
					isActive
						? "shadow-destructive/30 hover:bg-destructive/90"
						: "hover:bg-accent hover:text-accent-foreground border-primary/20"
				}`}
			>
				<Power className="h-8 w-8 md:h-10 md:w-10" />
				<span>{isActive ? "STOP" : "START"}</span>
			</Button>
		</div>

		<div className="mt-6 flex items-center gap-2 text-sm text-muted-foreground">
			{isActive ? (
				<>
					<ShieldAlert className="h-4 w-4 text-destructive" />
					<span>Suppressing 2.4 GHz Bluetooth advertising channels...</span>
				</>
			) : (
				<>
					<ShieldCheck className="h-4 w-4 text-emerald-500" />
					<span>Airspace clear. Ready to deploy.</span>
				</>
			)}
		</div>
	</div>
);
