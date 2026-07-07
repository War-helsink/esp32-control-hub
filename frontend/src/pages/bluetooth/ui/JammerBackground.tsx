interface JammerBackgroundProps {
	isActive: boolean;
}

export const JammerBackground: React.FC<JammerBackgroundProps> = ({
	isActive,
}) => (
	<div className="absolute inset-0 z-0 overflow-hidden pointer-events-none">
		<div
			className={`absolute -top-40 -right-40 w-96 h-96 rounded-full blur-[128px] transition-colors duration-1000 ${isActive ? "bg-destructive/20" : "bg-primary/10"}`}
		/>
		<div
			className={`absolute -bottom-20 -left-20 w-80 h-80 rounded-full blur-[96px] transition-colors duration-1000 ${isActive ? "bg-destructive/10" : "bg-muted/20"}`}
		/>
	</div>
);
