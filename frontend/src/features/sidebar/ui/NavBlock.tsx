import { Fragment } from "react";
import { SidebarGroupLabel } from "@/shared/ui";
import type { NavItemType } from "../types";
import { NavItem } from "./NavItem";

interface NavBlockProps {
	pathname: string;
	title: string;
	items: NavItemType[];
}

export const NavBlock: React.FC<NavBlockProps> = ({
	title,
	items,
	pathname,
}) => {
	return (
		<Fragment>
			<SidebarGroupLabel>{title}</SidebarGroupLabel>
			{items.map((item) => (
				<NavItem key={item.title} item={item} pathname={pathname} />
			))}
		</Fragment>
	);
};
