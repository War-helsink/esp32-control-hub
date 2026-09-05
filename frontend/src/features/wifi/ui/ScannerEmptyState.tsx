import type { LucideIcon } from "lucide-react";

interface ScannerEmptyStateProps {
	icon: LucideIcon;
	title: string;
	description: string;
}

export const ScannerEmptyState: React.FC<ScannerEmptyStateProps> = ({
	icon: Icon,
	title,
	description,
}) => {
	return (
		<div className="flex min-h-52 flex-col items-center justify-center px-6 py-10 text-center">
			<div className="mb-4 flex size-12 items-center justify-center rounded-full bg-muted">
				<Icon className="size-5 text-muted-foreground" />
			</div>

			<h3 className="font-medium">{title}</h3>

			<p className="mt-1 max-w-sm text-sm text-muted-foreground">
				{description}
			</p>
		</div>
	);
};
