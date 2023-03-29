<template>
  <div class="row text-h5">
    <q-card class="col">
      <q-card-section>
        <div>
          注册账号数量:
          {{ gameInfo.account }}
        </div>

        <div>
          游戏角色数量:
          {{ gameInfo.character }}
        </div>

        <div>
          公会数量:
          {{ gameInfo.guild }}
        </div>

        <div>
          市场在售数量:
          {{ gameInfo.market }}
        </div>
      </q-card-section>
    </q-card>

    <div class="col-auto" style="width: 8px" />

    <q-card class="col">
      <q-card-section>
        <div>
          服务器数量:
          {{ gameInfo.online.world }}
        </div>

        <div>
          线路数量:
          {{ gameInfo.online.server }}
        </div>

        <div>
          在线数量（已登录）:
          {{ gameInfo.online.loginning }}
        </div>

        <div>
          在线数量（游玩中）:
          {{ gameInfo.online.playing }}
        </div>
      </q-card-section>
    </q-card>
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue';
import { useQuasar } from 'quasar';
import { useStore } from 'stores/store';
import useFetch from 'src/components/fetch';

const $q = useQuasar();
const store = useStore();

const gameInfo = ref({
  account: 0,
  character: 0,
  guild: 0,
  market: 0,
  online: {
    world: 0,
    server: 0,
    loginning: 0,
    playing: 0,
  },
});

useFetch()
  .get(store.backend + '/api/status/overview', $q.cookies.get('canplay_token'))
  .then((resp) => {
    if (resp.data.status === 1) {
      gameInfo.value.account = resp.data.msg.account;
      gameInfo.value.character = resp.data.msg.character;
      gameInfo.value.guild = resp.data.msg.guild;
      gameInfo.value.market = resp.data.msg.market;

      let world = [],
        server = [];
      for (let index = 0; index < resp.data.msg.online.length; index++) {
        const element = resp.data.msg.online[index];

        if (world.indexOf(element.world) < 0) {
          world.push(element.world);
          gameInfo.value.online.world += 1;
        }

        if (server.indexOf(element.server) < 0) {
          server.push(element.server);
          gameInfo.value.online.server += 1;
          gameInfo.value.online.loginning += element.loginning;
          gameInfo.value.online.playing += element.playing;
        }
      }
      world = server = [];
    }
  })
  .catch(() => {
    $q.notify('网络错误，请稍后重试');
  });
</script>
