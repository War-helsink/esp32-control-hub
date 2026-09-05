import { cn } from "@/shared/lib/utils";
import { getSignalLabel, getSignalLevel, getSignalTextClass } from "../utils";

interface SignalIndicatorProps {
	rssi: number;
	showValue?: boolean;
}

export const SignalIndicator: React.FC<SignalIndicatorProps> = ({
	rssi,
	showValue = true,
}) => {
	const level = getSignalLevel(rssi);
	const label = getSignalLabel(rssi);

	return (
		<div
			className="flex items-center gap-2"
			title={`${label} signal (${rssi} dBm)`}
		>
			<div className="flex h-5 items-end gap-0.5">
				{[1, 2, 3, 4].map((bar) => (
					<span
						key={bar}
						className={cn(
							"w-1 rounded-sm bg-muted-foreground/20 transition-colors",
							bar === 1 && "h-1.5",
							bar === 2 && "h-2.5",
							bar === 3 && "h-3.5",
							bar === 4 && "h-5",
							bar <= level && "bg-current",
							getSignalTextClass(rssi),
						)}
					/>
				))}
			</div>

			{showValue && (
				<span className="whitespace-nowrap text-xs text-muted-foreground">
					{rssi} dBm
				</span>
			)}
		</div>
	);
};
