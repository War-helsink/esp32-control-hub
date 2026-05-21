import { Link, useLocation } from "react-router";
import {
	SidebarGroup,
	SidebarGroupLabel,
	SidebarMenu,
	SidebarMenuButton,
	SidebarMenuItem,
} from "@/shared/ui";
import { navMain } from "../config";

export const NavMain: React.FC = () => {
	const { pathname } = useLocation();

	return (
		<>
			<SidebarGroup>
				<SidebarGroupLabel>Tools</SidebarGroupLabel>
				<SidebarMenu>
					{navMain.map((item) => (
						<SidebarMenuItem key={item.title}>
							<SidebarMenuButton asChild isActive={pathname === item.url}>
								<Link to={item.url}>
									<item.icon />
									<span>{item.title}</span>
								</Link>
							</SidebarMenuButton>
						</SidebarMenuItem>
					))}
				</SidebarMenu>
			</SidebarGroup>
			<SidebarGroup>
				<SidebarGroupLabel>Settings</SidebarGroupLabel>
				<SidebarMenu>
					<SidebarMenuItem></SidebarMenuItem>
				</SidebarMenu>
			</SidebarGroup>
		</>
	);
};
