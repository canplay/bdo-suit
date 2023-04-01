import { useStore } from 'src/stores/store';

export default function usePermission() {
  const store = useStore();

  const check = (arg: string) => {
    let ret = false;

    if (store.user.permission === null || store.user.permission === undefined) {
      return ret;
    }

    if (store.user.permission[arg]) ret = true;

    return ret;
  };

  return { check };
}
