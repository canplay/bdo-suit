import { defineStore } from 'pinia';

export const useStore = defineStore('global', {
  state: () => ({
    // backend: '/backend',
    backend: 'http://1.13.22.82:51530',
    // backend: 'http://127.0.0.1:51530',
    user: {
      characters: [] as any,
      signin: false,
      userno: -1,
      username: '',
      password: '',
      familyname: '',
      cash: 0,
      pearl: 0,
      permission: {} as any,
    },
    admin: [] as any,
    dialog: {
      items: false,
    },
  }),
});
