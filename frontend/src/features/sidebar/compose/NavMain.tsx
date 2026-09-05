import { useLocation } from "react-router";
import { SidebarGroup, SidebarGroupLabel, SidebarMenu } from "@/shared/ui";
import { NavBlock } from "../ui/NavBlock";
import { NavItem } from "../ui/NavItem";
import { navMain } from "../config";
import { isNavBlock } from "../utils";

export const NavMain: React.FC = () => {
	const { pathname } = useLocation();

	return (
		<SidebarGroup>
			<SidebarGroupLabel>Tools</SidebarGroupLabel>
			<SidebarMenu>
				{navMain.map((item) =>
					isNavBlock(item) ? (
						<NavBlock
							key={item.title}
							pathname={pathname}
							title={item.title}
							items={item.items}
						/>
					) : (
						<NavItem key={item.title} pathname={pathname} item={item} />
					),
				)}
			</SidebarMenu>
		</SidebarGroup>
	);
};
