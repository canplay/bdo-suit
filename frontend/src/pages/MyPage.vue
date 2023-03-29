<template>
  <q-page padding>
    <div class="row q-gutter-md">
      <q-card
        class="col-auto"
        style="width: 100%; max-width: 250px"
        v-for="item in characters"
        :key="item"
      >
        <q-img src="https://cdn.quasar.dev/img/parallax2.jpg" />

        <q-card-section>
          <div class="text-h6">角色名: {{ item.characterName }}</div>
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
        <q-form @submit="onSubmit">
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
        <q-form @submit="onSubmit">
          <div class="row">
            <q-input
              class="col"
              v-model="mail.title"
              label="标题"
              lazy-rules
              :rules="[(val) => (val && val.length > 0) || '此为必填项']"
            />

            <div class="col-auto" style="width: 8px" />

            <q-input
              class="col"
              v-model="mail.contents"
              label="内容"
              lazy-rules
              :rules="[(val) => (val && val.length > 0) || '此为必填项']"
            />

            <div class="col-auto" style="width: 8px" />

            <q-input class="col" v-model="mail.variousNo" label="物品" />
          </div>

          <div style="height: 8px" />

          <div class="row">
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
</template>

<script setup lang="ts">
import { ref } from 'vue';

const account = ref({
  username: '',
  password: '',
  familyName: '',
});
const characters = ref([] as any);
const mail = ref({
  title: '',
  contents: '',
  variousNo: '',
  enchantLevel: '0',
  itemCount: '0',
});

for (let index = 0; index < 5; index++) {
  characters.value.push({
    characterName: index,
    level: 1,
    classType: 1,
    totalPlayTime: 1,
    experience: 1,
    tendency: 1,
    variedWeight: 1,
    inventorySlotCount: 1,
    hp: 1,
    mp: 1,
    sp: 1,
    wp: 1,
    titleKey: 1,
    defenceValue: 1,
    offenceValue: 1,
    awakenValue: 1,
    expanded: false,
  });
}

const onSubmit = () => {
  return;
};
</script>
