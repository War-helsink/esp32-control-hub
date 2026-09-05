export abstract class BaseApiService {
	protected static readonly BASE_PREFIX = "/api";

	protected static async requestJSON<T>(
		url: string,
		config: RequestInit,
	): Promise<T> {
		const response = await this.request(url, config);

		if (!response.ok) {
			throw new Error(
				`[API Error] Status: ${response.status} on ${config.method}`,
			);
		}

		if (response.status === 204) {
			return {} as T;
		}

		return response.json();
	}

	protected static async request(
		url: string,
		config: RequestInit,
	): Promise<Response> {
		const fullUrl = `${this.BASE_PREFIX}${url}`;

		return fetch(fullUrl, {
			...config,
			headers: {
				"Content-Type": "application/json",
				...config.headers,
			},
		});
	}

	protected static async get<T>(
		url: string,
		headers?: Record<string, string>,
	): Promise<T> {
		return this.requestJSON<T>(url, { method: "GET", headers });
	}

	protected static async post<T>(
		url: string,
		body?: any,
		headers?: Record<string, string>,
	): Promise<T> {
		return this.requestJSON<T>(url, {
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
		return this.requestJSON<T>(url, {
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
		return this.requestJSON<T>(url, {
			method: "PATCH",
			headers,
			body: body ? JSON.stringify(body) : undefined,
		});
	}

	protected static async delete<T>(
		url: string,
		headers?: Record<string, string>,
	): Promise<T> {
		return this.requestJSON<T>(url, { method: "DELETE", headers });
	}
}
