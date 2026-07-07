import { Play, Square, Loader2, Bluetooth, Trash2 } from "lucide-react";
import { Card, CardContent, CardHeader, CardTitle, Button } from "@/shared/ui";

interface ScannerControlsProps {
	isScanning: boolean;
	isLoading: boolean;
	buttonLabel: string;
	statusText: string;
	onToggle: () => void;
	onClear: () => void;
}

export const ScannerControls: React.FC<ScannerControlsProps> = ({
	isScanning,
	isLoading,
	buttonLabel,
	statusText,
	onToggle,
	onClear,
}) => (
	<Card>
		<CardHeader>
			<CardTitle className="flex items-center justify-between gap-2">
				<div className="flex items-center gap-2">
					<Bluetooth className="h-5 w-5" />
					Bluetooth scanner
				</div>
				<Button
					onClick={onClear}
					disabled={isScanning || isLoading}
					variant="outline"
				>
					<Trash2 />
				</Button>
			</CardTitle>
		</CardHeader>
		<CardContent className="flex flex-col gap-4">
			<Button
				onClick={onToggle}
				disabled={isLoading}
				className="w-fit gap-2"
				variant="default"
			>
				{isLoading ? (
					<Loader2 className="h-4 w-4 animate-spin" />
				) : isScanning ? (
					<Square className="h-4 w-4 fill-current" />
				) : (
					<Play className="h-4 w-4 fill-current text-green-500" />
				)}
				{buttonLabel}
			</Button>
			<p className="text-sm text-muted-foreground">{statusText}</p>
		</CardContent>
	</Card>
);
