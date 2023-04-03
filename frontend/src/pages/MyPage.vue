<template>
  <q-page padding>
    <div class="row q-gutter-md">
      <q-card
        class="col-auto"
        style="width: 100%; max-width: 400px"
        v-for="item in store.user.characters"
        :key="item"
      >
        <q-img src="imgs/logo.png" />

        <q-card-section>
          <div class="row">
            <div class="col text-h6">角色名: {{ item.characterName }}</div>
            <q-btn
              class="col"
              color="primary"
              label="自助修改"
              @click="onDialog(item)"
            />
          </div>

          <div class="row">
            <div class="col text-caption">等级: {{ item.level }}</div>
            <div class="col text-caption">
              游玩时长: {{ item.totalPlayTime }}
            </div>
          </div>
        </q-card-section>

        <q-separator />

        <q-card-section class="text-body1">
          <div class="row">
            <div class="col">经验值: {{ item.experience }}</div>
            <div class="col">善恶倾向: {{ item.tendency }}</div>
          </div>

          <div class="row">
            <div class="col">负重: {{ item.variedWeight }}</div>
            <div class="col">背包数量: {{ item.inventorySlotCount }}</div>
          </div>

          <div class="row">
            <div class="col">HP: {{ item.hp }}</div>
            <div class="col">MP: {{ item.mp }}</div>
          </div>

          <div class="row">
            <div class="col">SP: {{ item.sp }}</div>
            <div class="col">WP: {{ item.wp }}</div>
          </div>

          <div class="row">
            <div class="col">头衔: {{ item.titleKey }}</div>
            <div class="col">防御力: {{ item.defenceValue }}</div>
          </div>

          <div class="row">
            <div class="col">攻击力: {{ item.offenceValue }}</div>
            <div class="col">觉醒攻击力: {{ item.awakenValue }}</div>
          </div>
        </q-card-section>
      </q-card>
    </div>

    <div style="height: 8px" />

    <q-card>
      <q-card-section class="text-h6">自助修改</q-card-section>

      <q-card-section>
        <q-form @submit="onUpdateInfo">
          <div class="row">
            <q-input class="col" v-model="account.username" label="用户名" />

            <div class="col-auto" style="width: 8px" />

            <q-input class="col" v-model="account.password" label="密码" />

            <div class="col-auto" style="width: 8px" />

            <q-input class="col" v-model="account.familyName" label="家族名" />

            <div class="col-auto" style="width: 8px" />

            <q-btn class="col" color="primary" label="修改" type="submit" />
          </div>
        </q-form>
      </q-card-section>
    </q-card>

    <div style="height: 8px" />

    <q-card>
      <q-card-section class="text-h6">自助邮件</q-card-section>

      <q-card-section>
        <q-form @submit="onSendMail">
          <div class="row">
            <q-input class="col" v-model="mail.variousNo" label="物品ID">
              <template v-slot:append>
                <q-btn
                  color="primary"
                  label="ID列表"
                  @click="store.dialog.items = true"
                />
              </template>
            </q-input>

            <div class="col-auto" style="width: 8px" />

            <q-input class="col" v-model="mail.enchantLevel" label="强化等级" />

            <div class="col-auto" style="width: 8px" />

            <q-input class="col" v-model="mail.itemCount" label="物品数量" />

            <div class="col-auto" style="width: 8px" />

            <q-btn class="col" color="primary" label="发送" type="submit" />
          </div>
        </q-form>
      </q-card-section>
    </q-card>
  </q-page>

  <q-dialog v-model="dialog.show">
    <q-card>
      <q-card-section class="text-h6">修改角色名</q-card-section>

      <q-card-section>
        <q-input v-model="dialog.name" label="角色名" />

        <q-input v-model="dialog.x" label="坐标X" />

        <q-input v-model="dialog.y" label="坐标Y" />

        <q-input v-model="dialog.z" label="坐标Z" />
      </q-card-section>

      <q-card-actions>
        <q-btn
          class="fit"
          color="primary"
          label="确定"
          @click="onCharacterModify()"
        />
      </q-card-actions>
    </q-card>
  </q-dialog>

  <itemComp />
</template>

<script setup lang="ts">
import { onMounted, ref } from 'vue';
import { useQuasar } from 'quasar';
import useFetch from 'src/components/fetch';
import { useStore } from 'src/stores/store';
import itemComp from 'src/components/itemComp.vue';
import * as jose from 'jose';

const $q = useQuasar();
const store = useStore();

const account = ref({
  username: '',
  password: '',
  familyName: '',
});

const mail = ref({
  variousNo: '',
  enchantLevel: '0',
  itemCount: '0',
});

const dialog = ref({
  show: false,
  character: {} as any,
  name: '',
  x: '',
  y: '',
  z: '',
});

const onUpdateInfo = () => {
  let time = setTimeout(() => {
    $q.loading.hide();
    clearTimeout(time);
  }, 120000);

  $q.loading.show();

  useFetch()
    .post(
      store.backend + '/api/user/update',
      {
        userNo: store.user.userno,
        username: account.value.username,
        password: account.value.password,
        userNickname: account.value.familyName,
        isValid: 1,
        pcroom: 1,
        membershipType: 0,
      },
      $q.cookies.get('canplay_token')
    )
    .then((resp) => {
      $q.loading.hide();
      clearTimeout(time);

      if (resp.data.status === 0) {
        $q.notify(resp.data.msg);
        return;
      }

      refrashUserInfo();
      $q.notify('修改成功');
    })
    .catch(() => {
      $q.notify('网络错误，请稍后重试');
      $q.loading.hide();
      clearTimeout(time);
    });
};

const onSendMail = () => {
  let time = setTimeout(() => {
    $q.loading.hide();
    clearTimeout(time);
  }, 120000);

  $q.loading.show();

  useFetch()
    .post(
      store.backend + '/api/user/mail',
      {
        receiverName: store.user.username,
        receiverUserNo: store.user.userno,
        variousNo: parseInt(mail.value.variousNo),
        enchantLevel: parseInt(mail.value.enchantLevel),
        itemCount: parseInt(mail.value.itemCount),
      },
      $q.cookies.get('canplay_token')
    )
    .then((resp) => {
      $q.loading.hide();
      clearTimeout(time);

      if (resp.data.status === 0) {
        $q.notify(resp.data.msg);
        return;
      }

      $q.notify('发送邮件成功');
    })
    .catch(() => {
      $q.notify('网络错误，请稍后重试');
      $q.loading.hide();
      clearTimeout(time);
    });
};

const onDialog = (val: any) => {
  dialog.value.character = val;
  dialog.value.name = val.characterName;
  dialog.value.x = val.currentPositionX;
  dialog.value.y = val.currentPositionY;
  dialog.value.z = val.currentPositionZ;
  dialog.value.show = true;
};

const onCharacterModify = () => {
  let time = setTimeout(() => {
    $q.loading.hide();
    clearTimeout(time);
  }, 120000);

  $q.loading.show();

  useFetch()
    .post(
      store.backend + '/api/user/character/update',
      {
        characterNo: dialog.value.character.characterNo,
        characterName: dialog.value.name,
        currentPositionX: parseInt(dialog.value.x),
        currentPositionY: parseInt(dialog.value.y),
        currentPositionZ: parseInt(dialog.value.z),
      },
      $q.cookies.get('canplay_token')
    )
    .then((resp) => {
      $q.loading.hide();
      clearTimeout(time);

      if (resp.data.status === 0) {
        $q.notify(resp.data.msg);
        return;
      }

      refrashUserInfo();
      dialog.value.show = false;
      $q.notify('修改成功');
    })
    .catch(() => {
      $q.notify('网络错误，请稍后重试');
    });
};

const refrashUserInfo = () => {
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
        userno: resp.data.msg.userNo,
        signin: true,
        username: account.value.username,
        password: account.value.password,
        familyname: resp.data.msg.userNickname,
        cash: 0,
        pearl: 0,
        permission: resp.data.msg.permission,
        characters: resp.data.msg.characters,
      };
    })
    .catch(() => {
      $q.notify('网络错误，请稍后重试');
    });
};

onMounted(() => {
  account.value.username = store.user.username;
  account.value.password = store.user.password;
  account.value.familyName = store.user.familyname;
});
</script>
