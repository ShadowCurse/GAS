--################## Required extension for passwords ########################
create extension pgcrypto;

--################## Creating tables ########################
create table logs
(
    id          int generated always as identity primary key not null,
    date        timestamp                                    not null,
    description text                                         not null
);

create table users
(
    id          int generated always as identity primary key not null,
    username    text                                         not null unique,
    password    text                                         not null,
    email       text                                         not null,
    description text
);

create table resourcetype
(
    id   int generated always as identity primary key not null,
    name text                                         not null unique
);

create table resource
(
    id          int generated always as identity primary key not null,
    name        text                                         not null,
    description text,
    size        bigint                                       not null,
    type        int                                          not null,
    data        oid                                          not null,
    foreign key (type) references resourcetype (id) on delete cascade
);

create table dependency
(
    id                  int generated always as identity primary key not null,
    requesting_resource int                                          not null,
    required_resource   int                                          not null,
    foreign key (requesting_resource) references resource (id) on delete cascade,
    foreign key (required_resource) references resource (id) on delete cascade
);

create table commits
(
    id       int generated always as identity primary key not null,
    "user"     int                                          not null,
    resource int                                          not null,
    date     timestamp                                    not null,
    message  text,
    foreign key ("user") references users (id) on delete cascade,
    foreign key (resource) references resource (id) on delete cascade
);

--################## Deleting tables ########################

drop table commits;
drop table dependency;
drop table resource;
drop table resourcetype;
drop table users;
drop table logs;