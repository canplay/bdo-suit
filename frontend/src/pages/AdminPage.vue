<template>
  <q-page>
    <q-card square>
      <q-splitter
        v-model="splitterModel"
        style="height: calc(100vh - 56px - 50px)"
      >
        <template v-slot:before>
          <q-scroll-area class="fit">
            <q-tabs v-model="tab" vertical class="text-teal">
              <q-tab
                name="overview"
                v-if="usePermission().check('admin.overview')"
                icon="home"
                label="概览"
              />
              <q-tab
                name="web"
                v-if="usePermission().check('admin.web')"
                icon="analytics"
                label="前端管理"
              />
              <q-tab
                name="user"
                v-if="usePermission().check('admin.user')"
                icon="groups"
                label="用户管理"
              />
              <q-tab
                name="shop"
                v-if="usePermission().check('admin.shop')"
                icon="store"
                label="商城管理"
              />
              <q-tab
                name="system"
                v-if="usePermission().check('admin.system')"
                icon="settings"
                label="系统设置"
              />
            </q-tabs>
          </q-scroll-area>
        </template>

        <template v-slot:after>
          <q-scroll-area class="fit">
            <q-tab-panels
              v-model="tab"
              animated
              swipeable
              vertical
              transition-prev="jump-up"
              transition-next="jump-up"
            >
              <q-tab-panel name="overview">
                <overviewComp />
              </q-tab-panel>

              <q-tab-panel name="web">
                <webComp />
              </q-tab-panel>

              <q-tab-panel name="user">
                <userComp />
              </q-tab-panel>

              <q-tab-panel name="system">
                <systemComp />
              </q-tab-panel>
            </q-tab-panels>
          </q-scroll-area>
        </template>
      </q-splitter>
    </q-card>
  </q-page>
</template>

<script setup lang="ts">
import { ref } from 'vue';
import webComp from 'src/components/admin/webComp.vue';
import overviewComp from 'src/components/admin/overviewComp.vue';
import userComp from 'src/components/admin/userComp.vue';
import systemComp from 'src/components/admin/systemComp.vue';
import usePermission from 'src/components/permission';

const splitterModel = ref(8);
const tab = ref('overview');
</script>
