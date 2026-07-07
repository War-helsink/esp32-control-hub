import { Link } from "react-router";
import { Button } from "@/shared/ui";

export const NotFound: React.FC = () => {
	return (
		<div className="flex flex-col items-center justify-center min-h-dvh px-4 text-center md:px-6">
			<div className="max-w-md space-y-4">
				<h1 className="text-9xl font-bold tracking-tighter text-primary">
					404
				</h1>
				<h2 className="text-3xl font-bold tracking-tight sm:text-4xl">
					Oops! Page not found
				</h2>
				<p className="text-muted-foreground md:text-xl">
					The page you are looking for might have been removed, had its name
					changed, or is temporarily unavailable.
				</p>
				<div className="mt-6">
					<Button asChild>
						<Link to="/">Back to Home</Link>
					</Button>
				</div>
			</div>
		</div>
	);
};
