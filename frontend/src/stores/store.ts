import { defineStore } from 'pinia';

export const useStore = defineStore('global', {
  state: () => ({
    // backend: '/backend',
    backend: 'http://1.13.22.82:51530',
    // backend: 'http://127.0.0.1:51530',
    user: {
      signin: false,
      username: '',
      password: '',
      familyname: '',
      cash: 0,
      pearl: 0,
    },
    admin: [] as any,
  }),
});
