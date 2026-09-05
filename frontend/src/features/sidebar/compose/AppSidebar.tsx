import {
	Sidebar,
	SidebarContent,
	SidebarHeader,
	SidebarMenu,
	SidebarMenuButton,
	SidebarMenuItem,
	SidebarRail,
} from "@/shared/ui";

import { NavMain } from "./NavMain";

export const AppSidebar: React.FC = () => {
	return (
		<Sidebar variant="inset" collapsible="icon">
			<SidebarHeader>
				<SidebarMenu>
					<SidebarMenuItem>
						<SidebarMenuButton size="lg" asChild className="cursor-pointer">
							<div>
								<div className="flex aspect-square size-12 items-center justify-center rounded-lg bg-background">
									<img
										className="size-10 object-contain"
										src="/favicon.png"
										alt="Icon"
									/>
								</div>
								<div className="grid flex-1 text-left text-sm leading-tight">
									<span className="truncate font-semibold">Packet Bat</span>
									<span className="truncate text-xs">Tools</span>
								</div>
							</div>
						</SidebarMenuButton>
					</SidebarMenuItem>
				</SidebarMenu>
			</SidebarHeader>
			<SidebarContent>
				<NavMain />
			</SidebarContent>
			<SidebarRail />
		</Sidebar>
	);
};
