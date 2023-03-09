import { api } from 'src/boot/axios';

export default function useFetch() {
  const post = async (
    url: string,
    params?: object,
    auth?: string,
    contentType?: string
  ) => {
    let data: any;
    let error: unknown;

    try {
      const resp = await api.post(url, params, {
        headers: {
          Authorization:
            auth != undefined || auth != '' ? 'Bearer ' + auth : '',
          'content-type':
            contentType != undefined || contentType != '' ? contentType : '',
        },
      });
      data = resp.data;
    } catch (err) {
      error = err;
    }

    return { data, error };
  };

  const get = async (url: string, auth?: string, contentType?: string) => {
    let data: any;
    let error: unknown;

    try {
      const resp = await api.get(url, {
        headers: {
          Authorization:
            auth != undefined || auth != '' ? 'Bearer ' + auth : '',
          'content-type':
            contentType != undefined || contentType != '' ? contentType : '',
        },
      });
      data = resp.data;
    } catch (err) {
      error = err;
    }

    return { data, error };
  };

  return { post, get };
}
