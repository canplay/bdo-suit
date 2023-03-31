import { useStore } from 'src/stores/store';

export default function usePermission() {
  const store = useStore();

  const check = (arg: string, ...args: string[]) => {
    if (store.user.permission === null || store.user.permission === undefined) {
      return false;
    }

    const p1: any = store.user.permission[arg];

    for (let index = 0; index < args.length; index++) {
      const element = args[index];

      console.log(p1[element]);
      if (
        p1 === null ||
        p1 === undefined ||
        p1[element] === null ||
        p1[element] === undefined
      ) {
        return false;
      }

      const p2: any = p1[element];

      if (
        p2 === null ||
        p2 === undefined ||
        p2[element] === null ||
        p2[element] === undefined
      ) {
        return false;
      }
    }

    return true;
  };

  return { check };
}
