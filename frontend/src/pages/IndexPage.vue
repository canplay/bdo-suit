<template>
  <q-page padding>
    <q-scroll-area style="height: calc(100vh - 56px - 50px - 58px)">
      <div class="row" id="index">
        <q-carousel
          dark
          arrows
          animated
          infinite
          autoplay
          class="col rounded-borders"
          height="410px"
          v-model="slide.model"
        >
          <q-carousel-slide
            v-for="items in slide.list"
            :key="items"
            :name="items.title"
            :img-src="items.img"
          >
            <div class="absolute-bottom custom-caption">
              <div class="text-h2">{{ items.title }}</div>
              <div class="text-subtitle1">{{ items.desc }}</div>
            </div>
          </q-carousel-slide>
        </q-carousel>

        <div class="col-auto" style="width: 8px" />

        <q-card dark class="col" id="news">
          <q-scroll-area class="fit">
            <q-list dark v-for="items in news" :key="items">
              <q-item clickable @click="onNews(items)">
                <q-item-section>
                  <q-item-label>{{ items.title }}</q-item-label>
                  <q-item-label caption lines="2">
                    {{ items.desc }}
                  </q-item-label>
                </q-item-section>

                <q-item-section side top>
                  <q-item-label caption>
                    {{ items.date }}
                  </q-item-label>
                </q-item-section>
              </q-item>
            </q-list>
          </q-scroll-area>
        </q-card>
      </div>

      <div style="height: 8px" />

      <q-card dark>
        <q-card-section>
          <q-form @submit="onSignup" @reset="onSignin" class="row">
            <div class="col" v-if="!store.user.signin">
              <q-input
                dark
                filled
                v-model="username"
                :label="$t('username')"
                lazy-rules
                :rules="[(val) => (val && val.length > 0) || '不能为空']"
              />

              <q-input
                dark
                filled
                :type="isPwd ? 'password' : 'text'"
                v-model="password"
                :label="$t('password')"
                lazy-rules
                :rules="[(val) => (val && val.length > 0) || '不能为空']"
              >
                <template v-slot:append>
                  <q-icon
                    :name="isPwd ? 'visibility_off' : 'visibility'"
                    class="cursor-pointer"
                    @click="isPwd = !isPwd"
                  />
                </template>
              </q-input>
            </div>

            <div class="col" v-else>
              <div class="text-h4">用户名: {{ store.user.username }}</div>

              <div class="text-h4">家族名: {{ store.user.familyname }}</div>

              <div class="text-h4">虚拟币: {{ store.user.cash }}</div>

              <div class="text-h4">珍珠: {{ store.user.pearl }}</div>
            </div>

            <div class="col-auto" style="width: 8px" />

            <div class="col row" v-if="!store.user.signin">
              <q-btn
                dark
                flat
                class="col text-warning text-bold"
                size="48px"
                type="submit"
              >
                <div class="absolute-center">{{ $t('signup') }}</div>
                <div
                  class="fit relative-position"
                  style="
                    background-image: url(imgs/slide/5.jpg);
                    background-position: right bottom, left top;
                    background-repeat: no-repeat, no-repeat;
                    background-size: 100% 100%;
                    z-index: -1;
                    filter: blur(3px);
                  "
                />
              </q-btn>

              <div class="col-auto" style="width: 8px" />

              <q-btn
                dark
                flat
                class="col text-white text-bold"
                size="48px"
                type="reset"
              >
                <div class="absolute-center">{{ $t('signin') }}</div>
                <div
                  class="fit relative-position"
                  style="
                    background-image: url(imgs/slide/5.jpg);
                    background-position: right bottom, left top;
                    background-repeat: no-repeat, no-repeat;
                    background-size: 100% 100%;
                    z-index: -1;
                    filter: blur(3px);
                  "
                />
              </q-btn>
            </div>

            <div class="col row" v-else>
              <q-btn
                dark
                flat
                class="col text-positive text-bold"
                size="48px"
                type="submit"
              >
                <div class="absolute-center">{{ $t('signout') }}</div>
                <div
                  class="fit relative-position"
                  style="
                    background-image: url(imgs/slide/4.jpg);
                    background-position: right bottom, left top;
                    background-repeat: no-repeat, no-repeat;
                    background-size: cover;
                    z-index: -1;
                    filter: blur(3px);
                  "
                />
              </q-btn>
            </div>
          </q-form>
        </q-card-section>
      </q-card>

      <div style="height: 8px" />

      <div class="row" style="height: 300px">
        <q-btn dark flat id="shop" class="col" @click="onShop">
          <div
            class="fit relative-position"
            style="
              background-image: url(imgs/slide/2.jpg);
              background-position: right bottom, left top;
              background-repeat: no-repeat, no-repeat;
              background-size: cover;
              z-index: -1;
            "
          />
          <div class="absolute-bottom custom-caption">
            <div class="text-h2">{{ $t('shop.hot') }}</div>
            <div class="text-subtitle1">{{ $t('shop.hotDesc') }}</div>
          </div>
        </q-btn>
      </div>

      <div style="height: 8px" />

      <q-table
        dark
        separator="cell"
        :columns="system.columns"
        :rows="system.rows"
        row-key="class"
        hide-bottom
      >
        <template v-slot:top>
          <div class="fit text-center text-h5 text-bold">
            {{ $t('system.title') }}
          </div>
        </template>

        <template v-slot:header-cell="props">
          <q-th :props="props" style="font: bold 18px arial, sans-serif">
            {{ $t(props.col.label) }}
          </q-th>
        </template>

        <template v-slot:body="props">
          <q-tr :props="props">
            <q-td
              key="class"
              :props="props"
              style="font: bold 18px arial, sans-serif"
            >
              {{ $t(props.row.class) }}
            </q-td>

            <q-td key="min" :props="props">
              {{ props.row.min }}
            </q-td>

            <q-td key="rec" :props="props">
              {{ props.row.rec }}
            </q-td>

            <q-td key="rem" :props="props">
              {{ props.row.rem }}
            </q-td>

            <q-td key="max" :props="props">
              {{ props.row.max }}
            </q-td>
          </q-tr>
        </template>
      </q-table>

      <div style="height: 8px" />

      <div class="row">
        <q-btn
          dark
          flat
          class="col text-white text-bold"
          size="24px"
          @click="openURL(link.nvidia)"
        >
          <div class="absolute-center">{{ $t('nvidia') }}</div>
          <div
            class="fit relative-position"
            style="
              background-image: url(imgs/slide/5.jpg);
              background-position: right bottom, left top;
              background-repeat: no-repeat, no-repeat;
              background-size: cover;
              z-index: -1;
              filter: blur(3px);
            "
          />
        </q-btn>

        <div style="width: 8px" />

        <q-btn
          dark
          flat
          class="col text-white text-bold"
          size="24px"
          @click="openURL(link.amd)"
        >
          <div class="absolute-center">{{ $t('amd') }}</div>
          <div
            class="fit relative-position"
            style="
              background-image: url(imgs/slide/5.jpg);
              background-position: right bottom, left top;
              background-repeat: no-repeat, no-repeat;
              background-size: cover;
              z-index: -1;
              filter: blur(3px);
            "
          />
        </q-btn>
      </div>

      <div style="height: 8px" />

      <div class="row" id="downloads">
        <q-btn
          dark
          flat
          class="col text-white text-bold"
          size="42px"
          @click="openURL(link.client)"
        >
          <div class="absolute-center">{{ $t('downloads.client') }}</div>
          <div
            class="fit relative-position"
            style="
              background-image: url(imgs/slide/5.jpg);
              background-position: right bottom, left top;
              background-repeat: no-repeat, no-repeat;
              background-size: cover;
              z-index: -1;
              filter: blur(3px);
            "
          />
        </q-btn>

        <q-btn
          dark
          flat
          class="col text-white text-bold"
          size="42px"
          @click="openURL(link.launcher)"
        >
          <div class="absolute-center">{{ $t('downloads.launcher') }}</div>
          <div
            class="fit relative-position"
            style="
              background-image: url(imgs/slide/5.jpg);
              background-position: right bottom, left top;
              background-repeat: no-repeat, no-repeat;
              background-size: cover;
              z-index: -1;
              filter: blur(3px);
            "
          />
        </q-btn>
      </div>

      <div style="height: 8px" />

      <q-card dark class="text-h5 text-bold text-center" id="social">
        <q-card-section>
          <div>{{ $t('social') }}</div>

          <div style="height: 8px" />

          <div class="row">
            <q-btn
              dark
              flat
              square
              class="bg-white col"
              icon="img:imgs/weibo.svg"
            />
            <q-btn
              dark
              flat
              square
              class="bg-white col"
              icon="img:imgs/qq.svg"
            />
            <q-btn
              dark
              flat
              square
              class="bg-white col"
              icon="img:imgs/wechat.svg"
            />
            <q-btn
              dark
              flat
              square
              class="bg-white col"
              icon="img:imgs/github.svg"
            />
            <q-btn
              dark
              flat
              square
              class="bg-white col"
              icon="img:imgs/telegram.svg"
            />
            <q-btn dark flat class="bg-white col" icon="img:imgs/discord.svg" />
          </div>
        </q-card-section>
      </q-card>
    </q-scroll-area>
  </q-page>

  <q-dialog v-model="dialog.news.show">
    <q-card dark style="min-width: 80vw; height: 80vh">
      <q-card-section class="fit column">
        <div class="text-h4 text-bold">{{ dialog.news.title }}</div>

        <div style="height: 8px" />

        <div class="text-h6">{{ dialog.news.date }}</div>

        <div style="height: 8px" />

        <q-scroll-area class="col">
          <div v-html="dialog.news.content" />
        </q-scroll-area>
      </q-card-section>
    </q-card>
  </q-dialog>

  <q-dialog v-model="dialog.shop.show">
    <q-card dark style="min-width: 80vw; height: 80vh">
      <q-card-section class="fit">
        <q-table
          dark
          :rows="dialog.shop.rows"
          :columns="dialog.shop.columns"
          row-key="name"
          selection="multiple"
          v-model:selected="dialog.shop.selected"
          :filter="dialog.shop.filter"
          v-model:pagination="dialog.shop.pagination"
          grid
          hide-header
          class="fit"
        >
          <template v-slot:top>
            <div class="row fit">
              <div class="col text-h4 text-bold">{{ $t('shop.title') }}</div>

              <q-input
                class="col-auto"
                dark
                borderless
                dense
                debounce="300"
                v-model="dialog.shop.filter"
                :placeholder="$t('search')"
              >
                <template v-slot:append>
                  <q-icon name="search" />
                </template>
              </q-input>
            </div>
          </template>

          <template v-slot:item="props">
            <div
              class="q-pa-xs col-xs-12 col-sm-6 col-md-4 col-lg-3 grid-style-transition"
            >
              <q-card dark :class="props.selected ? 'bg-primary' : ''">
                <q-card-section>
                  <q-checkbox
                    dark
                    dense
                    v-model="props.selected"
                    :label="props.row.name"
                  />
                </q-card-section>

                <q-separator />

                <q-list dark>
                  <q-item v-for="col in props.cols" :key="col.name">
                    <q-item-section>
                      <q-item-label>{{ $t(col.label) }}</q-item-label>
                      <q-item-label caption>{{ col.value }}</q-item-label>
                    </q-item-section>
                  </q-item>
                </q-list>
              </q-card>
            </div>
          </template>
        </q-table>
      </q-card-section>
    </q-card>
  </q-dialog>
</template>

<style lang="sass" scoped>
.custom-caption
  text-align: center
  padding: 12px
  color: white
  background-color: rgba(0, 0, 0, .3)
</style>

<script setup lang="ts">
import { ref } from 'vue';
import { useQuasar, QTableProps, openURL } from 'quasar';
import { useStore } from 'src/stores/store';
import useFetch from 'components/fetch';
import { marked } from 'marked';
import { Base64 } from 'js-base64';

const $q = useQuasar();
const store = useStore();

const slide = ref({
  model: '1',
  list: [] as any,
});
const news = ref([] as any);
const username = ref('');
const password = ref('');
const isPwd = ref(true);

const link = ref({
  nvidia: '',
  amd: '',
  client: '',
  launcher: '',
  weibo: '',
  qq: '',
  wechat: '',
  github: '',
  telegram: '',
  discord: '',
});

const system = ref({
  columns: [
    {
      name: 'class',
      label: 'system.class',
      field: 'class',
      align: 'center',
      classes: 'text-bold',
    },
    {
      name: 'min',
      label: 'system.min',
      field: 'min',
      align: 'center',
    },
    {
      name: 'rec',
      label: 'system.rec',
      field: 'rec',
      align: 'center',
    },
    {
      name: 'rem',
      label: 'system.rem',
      field: 'rem',
      align: 'center',
    },
    {
      name: 'max',
      label: 'system.max',
      field: 'max',
      align: 'center',
    },
  ] as QTableProps['columns'],
  rows: [
    {
      class: 'system.cpu',
      min: 'Intel Core i3',
      rec: 'Intel Core i5',
      rem: 'Intel Core i7-8700',
      max: 'Intel Core i7-8700K',
    },
    {
      class: 'system.disk',
      min: '~41GB',
      rec: '~41GB',
      rem: '~41GB',
      max: '~41GB',
    },
    {
      class: 'system.ram',
      min: '4GB',
      rec: '8GB',
      rem: '16GB',
      max: '32GB',
    },
    {
      class: 'system.graphics',
      min: 'NVIDIA GTS 250、AMD HD 3870 X2',
      rec: 'NVIDIA GTX 970、AMD RX 480',
      rem: 'NVIDIA GTX 1070 8GB',
      max: 'NVIDIA GTX 1080ti 11GB',
    },
    {
      class: 'system.os',
      min: 'Windows 10 x64',
      rec: 'Windows 10 x64',
      rem: 'Windows 10 x64',
      max: 'Windows 10 x64',
    },
  ] as any,
});

const dialog = ref({
  news: {
    show: false,
    title: '',
    date: '',
    desc: '',
    content: '',
  },
  shop: {
    show: false,
    pagination: {
      page: 1,
      rowsNumber: 0,
      rowsPerPage: 10,
      sortBy: 'create_date',
      descending: true,
    } as QTableProps['pagination'],
    columns: [
      {
        name: 'name',
        label: 'shop.name',
        field: 'name',
        align: 'left',
      },
      {
        name: 'price',
        label: 'shop.price',
        field: 'price',
        align: 'left',
      },
      {
        name: 'storage',
        label: 'shop.storage',
        field: 'storage',
        align: 'left',
      },
      {
        name: 'desc',
        label: 'shop.desc',
        field: 'desc',
        align: 'left',
      },
      {
        name: 'date',
        label: 'shop.date',
        field: 'date',
        align: 'left',
      },
    ] as QTableProps['columns'],
    rows: [
      {
        name: '测试商品',
        price: 50.0,
        storage: 99,
        desc: '测试商品说明',
        date: '2023-2-19',
      },
    ] as any,
    filter: '',
    selected: [] as any,
  },
});

const onSignin = () => {
  let time = setTimeout(() => {
    $q.loading.hide();
    clearTimeout(time);
  }, 180000);

  $q.loading.show();

  useFetch()
    .post(store.backend + '/api/user/signin', {
      username: username.value,
      password: password.value,
    })
    .then((resp) => {
      if (resp.data.status === 0) {
        $q.loading.hide();
        clearTimeout(time);
        $q.notify(resp.data.msg);
        return;
      }

      $q.cookies.set('canplay_token', resp.data.msg.token);

      store.user = {
        signin: true,
        username: username.value,
        password: password.value,
        familyname: resp.data.msg.userNickname,
        cash: 0,
        pearl: 0,
      };

      $q.loading.hide();
      clearTimeout(time);
    })
    .catch(() => {
      $q.loading.hide();
      clearTimeout(time);
      $q.notify('网络错误，请稍后重试');
    });
};

const onSignup = () => {
  if (store.user.signin) {
    $q.cookies.remove('canplay_token');
    store.user = {
      signin: false,
      username: '',
      password: '',
      familyname: '',
      cash: 0,
      pearl: 0,
    };
  } else {
    let time = setTimeout(() => {
      $q.loading.hide();
      clearTimeout(time);
    }, 180000);

    $q.loading.show();

    useFetch()
      .post(store.backend + '/api/user/signup', {
        username: username.value,
        password: password.value,
      })
      .then((resp) => {
        if (resp.data.status === 0) {
          $q.loading.hide();
          clearTimeout(time);
          $q.notify(resp.data.msg);
          return;
        }

        $q.cookies.set('canplay_token', resp.data.msg.token);

        store.user = {
          signin: true,
          username: username.value,
          password: password.value,
          familyname: resp.data.msg.userNickname,
          cash: 0,
          pearl: 0,
        };

        $q.loading.hide();
        clearTimeout(time);
      })
      .catch(() => {
        $q.loading.hide();
        clearTimeout(time);
        $q.notify('网络错误，请稍后重试');
      });
  }
};

const onNews = (val: any) => {
  dialog.value.news = {
    show: true,
    title: val.title,
    date: val.date,
    desc: val.desc,
    content: val.content,
  };
};

const onShop = (val: any) => {
  dialog.value.shop.show = true;
};

useFetch()
  .get(store.backend + '/api/news/latest')
  .then((resp) => {
    if (resp.data.status != 0) {
      for (let i = 0; i < resp.data.msg.length; ++i) {
        news.value.push({
          id: resp.data.msg[i].id,
          title: resp.data.msg[i].title,
          date: resp.data.msg[i].date.replace(' +0800', ''),
          author: resp.data.msg[i].author,
          desc: Base64.decode(resp.data.msg[i].content),
          content: marked.parse(Base64.decode(resp.data.msg[i].content)),
          create_date: resp.data.msg[i].create_date.replace(' +0800', ''),
          create_user: resp.data.msg[i].create_user,
          create_id: resp.data.msg[i].create_id,
          update_date: resp.data.msg[i].update_date.replace(' +0800', ''),
          update_user: resp.data.msg[i].update_user,
          update_id: resp.data.msg[i].update_id,
        });
      }
    } else {
      $q.notify('网络错误，请稍后重试');
    }
  })
  .catch(() => {
    $q.notify('网络错误，请稍后重试');
  });

useFetch()
  .post(store.backend + '/api/slide/info', {
    curPage: 0,
    maxPage: 10,
    sortBy: 'create_date',
    descending: true,
  })
  .then((resp) => {
    if (resp.data.status != 0) {
      for (let i = 0; i < resp.data.msg.length; ++i) {
        if (resp.data.msg[i].id === '') break;

        slide.value.list.push({
          id: resp.data.msg[i].id,
          title: resp.data.msg[i].title,
          desc: resp.data.msg[i].desc,
          link: resp.data.msg[i].link,
          create_date: resp.data.msg[i].create_date.replace(' +0800', ''),
          create_user: resp.data.msg[i].create_user,
          create_id: resp.data.msg[i].create_id,
          update_date: resp.data.msg[i].update_date.replace(' +0800', ''),
          update_user: resp.data.msg[i].update_user,
          update_id: resp.data.msg[i].update_id,
          img: resp.data.msg[i].img,
        });
      }
    } else {
      $q.notify('网络错误，请稍后重试');
    }
  })
  .catch(() => {
    $q.notify('网络错误，请稍后重试');
  });

useFetch()
  .post(store.backend + '/api/link/info', {
    curPage: 0,
    maxPage: 10,
    sortBy: 'create_date',
    descending: true,
  })
  .then((resp) => {
    if (resp.data.status != 0) {
      for (let i = 0; i < resp.data.msg.length; ++i) {
        if (resp.data.msg[i].id === '') break;

        switch (resp.data.msg[i].title) {
          case 'nvidia':
            link.value.nvidia = resp.data.msg[i].link;
            break;
          case 'amd':
            link.value.amd = resp.data.msg[i].link;
            break;
          case 'client':
            link.value.client = resp.data.msg[i].link;
            break;
          case 'launcher':
            link.value.launcher = resp.data.msg[i].link;
            break;
          case 'weibo':
            link.value.weibo = resp.data.msg[i].link;
            break;
          case 'qq':
            link.value.qq = resp.data.msg[i].link;
            break;
          case 'wechat':
            link.value.wechat = resp.data.msg[i].link;
            break;
          case 'github':
            link.value.github = resp.data.msg[i].link;
            break;
          case 'telegram':
            link.value.telegram = resp.data.msg[i].link;
            break;
          case 'discord':
            link.value.discord = resp.data.msg[i].link;
            break;
        }
      }
    } else {
      $q.notify('网络错误，请稍后重试');
    }
  })
  .catch(() => {
    $q.notify('网络错误，请稍后重试');
  });
</script>
