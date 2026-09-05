import { useState } from "react";
import { ChevronDown, ChevronUp } from "lucide-react";
import {
	Card,
	CardContent,
	CardDescription,
	CardHeader,
	CardTitle,
	Badge,
	Button,
} from "@/shared/ui";
import { features } from "../config";

export const DashboardPage: React.FC = () => {
	const [expandedId, setExpandedId] = useState<string | null>(null);

	const toggleExpand = (id: string) => {
		setExpandedId(expandedId === id ? null : id);
	};

	return (
		<div className="p-6 space-y-6 max-w-5xl mx-auto">
			<div className="flex flex-col md:flex-row md:items-center md:justify-between gap-4 border-b pb-5">
				<div>
					<h1 className="text-3xl font-bold tracking-tight">
						Packet Bat Network Auditor
					</h1>
					<p className="text-muted-foreground mt-1">
						Core architecture overview and firmware module status tracking.
					</p>
				</div>
				<div className="flex gap-2">
					<Badge variant="outline" className="text-xs py-1 px-2">
						Platform: ESP32-S3
					</Badge>
					<Badge variant="outline" className="text-xs py-1 px-2">
						Framework: C++ / Arduino
					</Badge>
				</div>
			</div>

			<Card className="bg-muted/40">
				<CardHeader>
					<CardTitle className="text-lg">Educational Blueprint</CardTitle>
					<CardDescription>
						This UI template is engineered to visualize low-level Wi-Fi stack
						operations on Espressif chips. The modules listed below represent
						the core C++ architectural components of the library.
					</CardDescription>
				</CardHeader>
			</Card>

			<div className="grid gap-4 md:grid-cols-2">
				{features.map((item) => {
					const isExpanded = expandedId === item.id;
					return (
						<Card
							key={item.id}
							className="flex flex-col justify-between transition-all duration-200"
						>
							<CardHeader className="flex flex-row items-start justify-between space-y-0 pb-3">
								<div className="flex items-center space-x-3">
									<div className="p-2 bg-background rounded-lg border">
										{item.icon}
									</div>
									<div>
										<CardTitle className="text-base font-semibold">
											{item.title}
										</CardTitle>
										<div className="mt-1">
											{item.status === "ready" && (
												<Badge
													variant="default"
													className="bg-green-600 text-[10px] h-5"
												>
													Ready
												</Badge>
											)}
											{item.status === "in_development" && (
												<Badge
													variant="secondary"
													className="bg-amber-500 text-white text-[10px] h-5"
												>
													In Progress
												</Badge>
											)}
											{item.status === "planned" && (
												<Badge variant="outline" className="text-[10px] h-5">
													Planned
												</Badge>
											)}
										</div>
									</div>
								</div>
							</CardHeader>

							<CardContent className="space-y-3 grow">
								<p className="text-sm text-muted-foreground">
									{item.description}
								</p>

								{isExpanded && (
									<div className="mt-2 pt-3 border-t text-xs text-muted-foreground bg-muted/50 p-3 rounded-md font-mono leading-relaxed">
										<span className="font-bold text-foreground block mb-1">
											C++ Implementation Specifications:
										</span>
										{item.extendedDoc}
									</div>
								)}

								<Button
									variant="ghost"
									size="sm"
									className="w-full justify-between mt-2 h-8 text-xs text-muted-foreground hover:text-foreground"
									onClick={() => toggleExpand(item.id)}
								>
									{isExpanded ? "Hide specs" : "Show implementation specs"}
									{isExpanded ? (
										<ChevronUp className="h-3 w-3" />
									) : (
										<ChevronDown className="h-3 w-3" />
									)}
								</Button>
							</CardContent>
						</Card>
					);
				})}
			</div>

			<Card>
				<CardHeader className="pb-3">
					<CardTitle className="text-sm font-medium flex items-center gap-2">
						<span className="flex h-2 w-2 rounded-full bg-green-500 animate-pulse" />
						Debug Console (Awaiting ESP32 connection...)
					</CardTitle>
				</CardHeader>
				<CardContent>
					<div className="bg-zinc-950 text-zinc-400 font-mono text-xs p-4 rounded-lg h-36 overflow-y-auto space-y-1 border">
						<div className="text-zinc-600">
							Frontend running in standalone sandbox mode.
						</div>
						<div>[WEB] Client UI compiled successfully.</div>
						<div>
							[WEB] Listening for server-sent events at
							`EventSource(\"/events\")`...
						</div>
					</div>
				</CardContent>
			</Card>
		</div>
	);
};
