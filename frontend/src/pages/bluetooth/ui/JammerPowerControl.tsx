import { Sliders } from "lucide-react";
import { Slider } from "@/shared/ui";

interface JammerPowerControlProps {
	power: number;
	disabled: boolean;
	onPowerChange: (value: number[]) => void;
}

export const JammerPowerControl: React.FC<JammerPowerControlProps> = ({
	power,
	disabled,
	onPowerChange,
}) => (
	<div className="w-full space-y-4 backdrop-blur-md bg-card/40 border border-border/60 rounded-2xl p-6 shadow-sm">
		<div className="flex items-center justify-between">
			<div className="flex items-center gap-2">
				<Sliders className="h-4 w-4 text-muted-foreground" />
				<span className="text-sm font-medium tracking-wide text-muted-foreground uppercase">
					Transmission Output
				</span>
			</div>
			<span className="text-sm font-mono font-bold bg-muted px-2.5 py-0.5 rounded">
				{power} dBm
			</span>
		</div>

		<Slider
			value={[power]}
			onValueChange={onPowerChange}
			min={0}
			max={9}
			step={1}
			disabled={disabled}
			className="py-2"
		/>
		<div className="flex justify-between text-[10px] uppercase tracking-wider text-muted-foreground/60 font-mono">
			<span>Min Power</span>
			<span>Max Suppressing</span>
		</div>
	</div>
);
