import { Activity, Server } from "lucide-react";

import { Badge, Tooltip, TooltipContent, TooltipTrigger } from "@/shared/ui";
import { useStatusStore } from "@/entities/status";
import { getActionLabel, getModuleIcon } from "../utils";
import { moduleLabels } from "../config";

export const DeviceStatus: React.FC = () => {
	const status = useStatusStore((state) => state.status);

	const activeProcess = useStatusStore((state) => state.activeProcess);

	const isRunning = status === "running" && activeProcess !== null;

	if (!isRunning) {
		return (
			<div className="flex h-full items-center justify-end px-4">
				<Tooltip>
					<TooltipTrigger asChild>
						<div className="flex cursor-default items-center gap-2">
							<div className="relative flex size-2">
								<span className="absolute inline-flex size-full animate-ping rounded-full bg-emerald-400 opacity-75" />

								<span className="relative inline-flex size-2 rounded-full bg-emerald-500" />
							</div>

							<div className="hidden items-center gap-2 sm:flex">
								<Server className="size-4 text-muted-foreground" />

								<span className="text-sm font-medium">ESP32</span>

								<Badge
									variant="secondary"
									className="text-emerald-700 dark:text-emerald-400"
								>
									Ready
								</Badge>
							</div>
						</div>
					</TooltipTrigger>

					<TooltipContent>ESP32 is ready</TooltipContent>
				</Tooltip>
			</div>
		);
	}

	const ModuleIcon = getModuleIcon(activeProcess.module);

	const moduleLabel = moduleLabels[activeProcess.module];

	const actionLabel = getActionLabel(activeProcess);

	return (
		<div className="flex h-full items-center justify-end px-4">
			<Tooltip>
				<TooltipTrigger asChild>
					<div className="flex min-w-0 cursor-default items-center gap-2">
						<div className="relative flex size-2 shrink-0">
							<span className="absolute inline-flex size-full animate-ping rounded-full bg-amber-400 opacity-75" />

							<span className="relative inline-flex size-2 rounded-full bg-amber-500" />
						</div>

						<ModuleIcon className="size-4 shrink-0 text-muted-foreground" />

						<div className="hidden min-w-0 items-center gap-2 sm:flex">
							<span className="text-sm font-medium">{moduleLabel}</span>

							<Badge variant="outline" className="max-w-44 gap-1.5">
								<Activity className="size-3 animate-pulse" />

								<span className="truncate">{actionLabel}</span>
							</Badge>
						</div>
					</div>
				</TooltipTrigger>

				<TooltipContent>
					{moduleLabel}: {actionLabel}
				</TooltipContent>
			</Tooltip>
		</div>
	);
};
