<template>
  <q-layout
    view="hHh LpR fFf"
    style="
      background-image: url(imgs/bg.jpg);
      background-position: right bottom, left top;
      background-repeat: no-repeat, no-repeat;
      background-size: 100% 100%;
    "
  >
    <q-header elevated>
      <q-toolbar class="bg-grey-10 text-white row">
        <q-img class="col-auto" src="imgs/logo.png" width="240px" />

        <div class="col-auto" style="width: 30px" />

        <q-tabs class="col" v-model="tab">
          <q-route-tab name="index" :label="$t('index')" to="/" />
          <q-route-tab
            name="news"
            :label="$t('news')"
            @click="onGoto('news')"
          />
          <q-route-tab
            name="shop"
            :label="$t('shop.title')"
            @click="onGoto('shop')"
          />
          <q-route-tab
            name="downloads"
            :label="$t('downloads.title')"
            @click="onGoto('downloads')"
          />
          <q-route-tab
            name="social"
            :label="$t('social')"
            @click="onGoto('social')"
          />
        </q-tabs>

        <div class="col-auto" style="width: 30px" />

        <q-btn
          v-if="store.user.familyname === 'WebAdmin'"
          flat
          label="管理面板"
          to="/admin"
        />

        <q-btn
          v-if="store.user.familyname === 'WebAdmin'"
          flat
          label="个人信息"
          to="/my"
        />

        <div class="col-auto" style="width: 30px" />

        <q-select
          dark
          class="col-auto"
          v-model="locale"
          :options="localeOptions"
          label="Global"
          map-options
          emit-value
        />
      </q-toolbar>
    </q-header>

    <q-footer elevated>
      <q-toolbar class="bg-grey-10 text-white text-center">
        <q-toolbar-title style="font-size: medium">
          Powered by CaNplay
        </q-toolbar-title>
      </q-toolbar>
    </q-footer>

    <q-page-container>
      <router-view />
    </q-page-container>
  </q-layout>
</template>

<script setup lang="ts">
import { ref } from 'vue';
import { useI18n } from 'vue-i18n';
import { useQuasar } from 'quasar';
import { useStore } from 'src/stores/store';
import useFetch from 'src/components/fetch';
import * as jose from 'jose';

const $q = useQuasar();
const store = useStore();

const { locale } = useI18n({ useScope: 'global' });
locale.value = navigator.language;
const localeOptions = [
  { value: 'en-US', label: 'English' },
  { value: 'zh-CN', label: '简体中文' },
];

const tab = ref('index');

const onGoto = (val: string) => {
  document.getElementById(val)?.scrollIntoView();
};

if ($q.cookies.has('canplay_token') && $q.cookies.get('canplay_token') != '') {
  const id = jose.decodeJwt($q.cookies.get('canplay_token')).id;

  useFetch()
    .get(
      store.backend + '/api/user/info/' + id,
      $q.cookies.get('canplay_token')
    )
    .then((resp) => {
      if (resp.data.status === 0) {
        $q.notify(resp.data.msg);
        return;
      }

      let username = resp.data.msg.userId.split(',');

      store.user = {
        signin: true,
        username: username[0],
        password: username[1],
        familyname: resp.data.msg.userNickname,
        cash: 0,
        pearl: 0,
      };
    })
    .catch(() => {
      $q.notify('网络错误，请稍后重试');
    });
}
</script>
