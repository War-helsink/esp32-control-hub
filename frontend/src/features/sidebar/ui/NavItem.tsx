import { Link } from "react-router";
import { SidebarMenuButton, SidebarMenuItem } from "@/shared/ui";
import type { NavItemType } from "../types";

interface NavItemProps {
	item: NavItemType;
	pathname: string;
}

export const NavItem: React.FC<NavItemProps> = ({ item, pathname }) => {
	return (
		<SidebarMenuItem key={item.title}>
			<SidebarMenuButton asChild isActive={pathname === item.url}>
				<Link to={item.url}>
					<item.icon />
					<span>{item.title}</span>
				</Link>
			</SidebarMenuButton>
		</SidebarMenuItem>
	);
};
