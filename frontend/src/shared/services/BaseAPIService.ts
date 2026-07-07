export abstract class BaseApiService {
	protected static readonly BASE_PREFIX = "/api";

	private static async request<T>(
		url: string,
		config: RequestInit,
	): Promise<T> {
		const fullUrl = `${this.BASE_PREFIX}${url}`;

		const response = await fetch(fullUrl, {
			...config,
			headers: {
				"Content-Type": "application/json",
				...config.headers,
			},
		});

		if (!response.ok) {
			throw new Error(
				`[API Error] Status: ${response.status} on ${config.method} ${fullUrl}`,
			);
		}

		if (response.status === 204) {
			return {} as T;
		}

		return response.json();
	}

	protected static async get<T>(
		url: string,
		headers?: Record<string, string>,
	): Promise<T> {
		return this.request<T>(url, { method: "GET", headers });
	}

	protected static async post<T>(
		url: string,
		body?: any,
		headers?: Record<string, string>,
	): Promise<T> {
		return this.request<T>(url, {
			method: "POST",
			headers,
			body: body ? JSON.stringify(body) : undefined,
		});
	}

	protected static async put<T>(
		url: string,
		body?: any,
		headers?: Record<string, string>,
	): Promise<T> {
		return this.request<T>(url, {
			method: "PUT",
			headers,
			body: body ? JSON.stringify(body) : undefined,
		});
	}

	protected static async patch<T>(
		url: string,
		body?: any,
		headers?: Record<string, string>,
	): Promise<T> {
		return this.request<T>(url, {
			method: "PATCH",
			headers,
			body: body ? JSON.stringify(body) : undefined,
		});
	}

	protected static async delete<T>(
		url: string,
		headers?: Record<string, string>,
	): Promise<T> {
		return this.request<T>(url, { method: "DELETE", headers });
	}
}
