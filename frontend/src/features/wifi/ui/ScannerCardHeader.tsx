import { CardHeader, CardTitle } from "@/shared/ui";

interface ScannerCardHeaderProps {
	title: string;
	start?: React.ReactNode;
	end?: React.ReactNode;
}

export const ScannerCardHeader: React.FC<ScannerCardHeaderProps> = ({
	title,
	start,
	end,
}) => {
	return (
		<CardHeader className="gap-4 border-b bg-muted/20 flex sm:flex-row sm:items-center sm:justify-between">
			<div className="flex flex-wrap items-center gap-2">
				<CardTitle>{title}</CardTitle>

				{start}
			</div>

			<div>{end}</div>
		</CardHeader>
	);
};
